/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <jni.h>
#include <jvmti.h>
#include <dlfcn.h>
#include <jmm.h>
#include "vm.h"
#include "gc.h"

JavaVM* VM::_vm;
jvmtiEnv* VM::_jvmti;
void* VM::_libjvm;
JVM_GetManagement VM::_jvmGetManagement;

void VM::Init(JavaVM *vm) {
    if (_jvmti != nullptr)
        return;

    _vm = vm;
    _vm->GetEnv((void **) &_jvmti, JVMTI_VERSION_1_0);

    jvmtiCapabilities capabilities = {0};
    capabilities.can_generate_garbage_collection_events = 1;
    _jvmti->AddCapabilities(&capabilities);

    jvmtiEventCallbacks callbacks = {nullptr};
    callbacks.GarbageCollectionStart = GcEvent::GarbageCollectionStart;
    callbacks.GarbageCollectionFinish = GcEvent::GarbageCollectionFinish;
    _jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));

    _jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_GARBAGE_COLLECTION_START, nullptr);
    _jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_GARBAGE_COLLECTION_FINISH, nullptr);

    _libjvm = getLibraryHandle("libjvm.so");

    _jvmGetManagement = (JVM_GetManagement) dlsym(_libjvm, "JVM_GetManagement");
}

void VM::HeapDump(const char *path) {
    JNIEnv *jni;
    _vm->AttachCurrentThread((void **) &jni, nullptr);

    auto *jmm = (JmmInterface *) _jvmGetManagement(JMM_VERSION_1_0);

    char buf[4096];
    snprintf(buf, sizeof(buf), "%s/dump_%u.hprof", path, pid());

    jmm->DumpHeap0(jni, jni->NewStringUTF(buf), JNI_FALSE);
    _vm->DetachCurrentThread();
}

void *VM::getLibraryHandle(const char *name) {
#ifdef __linux__
    void* handle = dlopen(name, RTLD_LAZY);
    if (handle != NULL) {
        return handle;
    }
    std::cerr << "Failed to load " << name << ": " << dlerror() << std::endl;
#endif
    return RTLD_DEFAULT;
}
