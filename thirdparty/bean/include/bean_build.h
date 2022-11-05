//
// Created by 叶盛 on 2022/10/26.
//

#ifndef OFDB_BEAN_BUILD_H
#define OFDB_BEAN_BUILD_H

#pragma once
#include <dlfcn.h>
#include "bean_creator.h"
namespace bean {
    inline void GetRegistBean(std::unordered_map<std::string, std::vector<std::string>> &dst) {
        auto pBeanFactory = BeanFactoryWrap::Instance();
        pBeanFactory->GetRegistBean(dst);
    }

    class BeanBuild {
    public:
        template<typename...Targs>
        static Object *NewSharedObject(const std::string &strActorName, Targs &&... args) {
            auto pBeanFactory = BeanFactoryWrap::Instance()->GetBeanFactory<Targs...>();
            Object* pActor = pBeanFactory->Create(typeid(BeanBuild).name(), strActorName, std::forward<Targs>(args)...);
            if (nullptr == pActor)
            {
                fprintf(stderr, "failed to make shared actor \"%s\"\n", strActorName.c_str());
                return(nullptr);
            }
            return pActor;
        }

        template<typename...Targs>
        static std::shared_ptr<Object> MakeSharedObject(const std::string &strActorName, Targs &&... args) {
            Object* pActor = NewSharedObject(strActorName, std::forward<Targs>(args)...);
            std::shared_ptr<Object> pSharedActor;
            pSharedActor.reset(pActor);
            pActor = nullptr;
            return pSharedActor;
        }

        template<typename...Targs>
        static std::shared_ptr<Object> MakeSharedObjectFromSo(const std::string &strSoName, const std::string &strActorName, Targs &&... args) {
            void* handle = dlopen(strSoName.c_str(), RTLD_LAZY);
            if (!handle) {
                fprintf(stderr, "%s\n", dlerror());
                return nullptr;
            }
            return MakeSharedObject(strActorName, std::forward<Targs>(args)...);
        }

        template<typename...Targs>
        static Object *NewSharedObjectFromSo(const std::string &strSoName, const std::string &strActorName, Targs &&... args) {
            void* handle = dlopen(strSoName.c_str(), RTLD_LAZY);
            if (!handle) {
                fprintf(stderr, "%s\n", dlerror());
                return nullptr;
            }
            return NewSharedObject(strActorName, std::forward<Targs>(args)...);
        }

        template<typename T>
        static std::shared_ptr<T> CostShared(std::shared_ptr<Object> pSharedActor) {
            if (pSharedActor->TypeID() == typeid(T).name()) {
                return std::dynamic_pointer_cast<T>(pSharedActor);
            }
            return nullptr;
        }

        template<typename T>
        static T *CostShared(Object *pSharedActor) {
            if (pSharedActor->TypeID() == typeid(T).name()) {
                return std::dynamic_pointer_cast<T>(pSharedActor);
            }
            return nullptr;
        }

        template<typename T, typename...Targs>
        static T *NewShared(const std::string &strSoName, const std::string &strActorName, Targs &&... args) {
            Object *pSharedActor;
            if (strSoName.empty()) {
                pSharedActor = NewSharedObject(strActorName, std::forward<Targs>(args)...);
            }
            else {
                pSharedActor = NewSharedObjectFromSo(strSoName, strActorName, std::forward<Targs>(args)...);
            }
            if (pSharedActor) {
                return CostShared<T>(pSharedActor);
            }
            return nullptr;
        }

        template<typename T, typename...Targs>
        static std::shared_ptr<T> MakeShared(const std::string &strSoName, const std::string &strActorName, Targs &&... args) {
            std::shared_ptr<Object> pSharedActor;
            if (strSoName.empty()) {
                pSharedActor = MakeSharedObject(strActorName, std::forward<Targs>(args)...);
            }
            else {
                pSharedActor = MakeSharedObjectFromSo(strSoName, strActorName, std::forward<Targs>(args)...);
            }
            if (pSharedActor) {
                return CostShared<T>(pSharedActor);
            }
            return nullptr;
        }
    };
}

#endif //OFDB_BEAN_BUILD_H
