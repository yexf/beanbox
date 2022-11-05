//
// Created by 叶盛 on 2022/10/25.
//

#ifndef OFDB_BEAN_CREATOR_H
#define OFDB_BEAN_CREATOR_H

#pragma once
#include <string>
#include <memory>
#include <cxxabi.h>
#include "bean_factory.h"
namespace bean {

    template<typename T, typename...Targs>
    class BeanCreator {
    public:
        inline void do_nothing() const { };
        BeanCreator()
        {
            char* szDemangleName = nullptr;
            std::string strTypeName;
#ifdef __GNUC__
            szDemangleName = abi::__cxa_demangle(typeid(T).name(), NULL, NULL, NULL);
#else
            //in this format?:     szDemangleName =  typeid(T).name();
                szDemangleName = abi::__cxa_demangle(typeid(T).name(), NULL, NULL, NULL);
#endif
            if (NULL != szDemangleName)
            {
                strTypeName = szDemangleName;
                free(szDemangleName);
            }
            m_strDemangleName = strTypeName;
            auto pBeanFactory = BeanFactoryWrap::Instance()->GetBeanFactory<Targs...>();
            pBeanFactory->Regist(typeid(this).name(), strTypeName, CreateObject);
        }
        virtual ~BeanCreator(){
            auto pBeanFactory = BeanFactoryWrap::Instance()->GetBeanFactory<Targs...>();
            pBeanFactory->UnRegist(typeid(this).name(), m_strDemangleName);
        };

        static T* CreateObject(Targs&&... args)
        {
            T* pT = nullptr;
            try
            {
                pT = new T(std::forward<Targs>(args)...);
            }
            catch(std::bad_alloc& e)
            {
                fprintf(stderr, "%s %d %s %s", __FILE__, __LINE__, __FUNCTION__, e.what());
                return(nullptr);
            }
            return pT;
        }

        const std::string &GetDemangleName () const {
            return m_strDemangleName;
        }

    private:
        std::string m_strDemangleName;
    };

}

#endif //OFDB_BEAN_CREATOR_H
