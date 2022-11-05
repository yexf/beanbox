//
// Created by 叶盛 on 2022/10/25.
//

#ifndef OFDB_BEAN_H
#define OFDB_BEAN_H

#pragma once
#include <vector>
#include <string>
#include <memory>
#include "bean_creator.h"
#include "bean_build.h"

namespace bean {
#define BEAN(Obj)           template<typename T, typename...Targs> \
                            class Obj : public BeanBase<bean::Obj, T, Targs...> {};

#define MAKESHARED(Obj)     template<typename...Targs> \
                            static std::shared_ptr<bean::Obj> MakeShared##Obj(const std::string &strSoName, \
                                                                const std::string &strActorName, Targs &&... args) { \
                                std::shared_ptr<bean::Obj> ret = BeanBuild::MakeShared<bean::Obj>(strSoName, strActorName, std::forward<Targs>(args)...); \
                                BeanFactoryWrap::Instance()->SetObject(strActorName, ret); return ret;} \
                            static std::shared_ptr<bean::Obj> GetShared##Obj(const std::string &strActorName) { \
                                std::shared_ptr<bean::Object> pSharedActor = BeanFactoryWrap::Instance()->GetObject(strActorName); \
                                if (pSharedActor) { return BeanBuild::CostShared<bean::Obj>(pSharedActor); }  return nullptr;} \
                            static void DeleteShared##Obj(const std::string &strActorName) { \
                                BeanFactoryWrap::Instance()->DelObject(strActorName); }

class Bean {
public:
    template<typename T, typename...Targs>
    class BeanBase : public T {
    public:
        BeanBase() {
            s_oRegister.do_nothing();
        }
        static const std::string &GetDemangleName() {
            return s_oRegister.GetDemangleName();
        }
    private:
        static BeanCreator<Targs...> s_oRegister;
    };

    //模版类的偏特化实现不同的对象继承
    BEAN(Object)
    MAKESHARED(Object)

    BEAN(Client)
    MAKESHARED(Client)

    BEAN(Server)
    MAKESHARED(Server)
};


template<typename T, typename...Targs>
BeanCreator<Targs...> Bean::BeanBase<T, Targs...>::s_oRegister;

}
#endif //OFDB_BEAN_H
