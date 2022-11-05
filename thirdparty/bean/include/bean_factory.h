//
// Created by 叶盛 on 2022/10/25.
//

#ifndef OFDB_BEAN_FACTORY_H
#define OFDB_BEAN_FACTORY_H


#include <functional>
#include <string>
#include <vector>
#include <mutex>
#include <memory>
#include <unordered_map>
#include "bean_object.h"
namespace bean {
    template<typename ...Targs>
    class BeanFactory;

    class BeanFactoryBase {
    public:
        BeanFactoryBase() {};

        virtual ~BeanFactoryBase() {};

        virtual std::string TypeID () = 0;

        virtual void GetRegistBeans(std::vector<std::string> &dst) = 0;

        static void SetHook(std::function<void(BeanFactoryBase*, const std::string &, const std::string &, const std::string &)> pHook) {
            m_pHook = pHook;
        }

        static void UnSetHook(std::function<void(BeanFactoryBase*, const std::string &, const std::string &, const std::string &)> pHook) {
            m_pHook = nullptr;
        }

    protected:
        static std::function<void(BeanFactoryBase*, const std::string &, const std::string &, const std::string &)> m_pHook;
    };

    class BeanFactoryWrap {
    public:
        static BeanFactoryWrap* Instance();

        template<typename ...Targs>
        std::shared_ptr<BeanFactory<Targs...>> GetBeanFactory() {
            const std::string type_name = typeid(BeanFactory<Targs...>).name();
            std::lock_guard<std::mutex> lock(m_Mutex);
            if (m_mapCreateFunction.count(type_name)) {
                return std::dynamic_pointer_cast<BeanFactory<Targs...>>(m_mapCreateFunction[type_name]);
            }
            else {
                std::shared_ptr<BeanFactory<Targs...>> pRet = std::make_shared<BeanFactory<Targs...>>();
                m_mapCreateFunction[type_name] = pRet;
                return pRet;
            }
        }

        void GetRegistBean(std::unordered_map<std::string, std::vector<std::string>> &dst) {
            std::lock_guard<std::mutex> lock(m_Mutex);
            for (auto &bean_factory : m_mapCreateFunction) {
                std::vector<std::string> temp;
                bean_factory.second->GetRegistBeans(temp);
                dst[bean_factory.first] = temp;
            }
        }

        void SetObject(const std::string &name, std::shared_ptr<Object> newObj);

        std::shared_ptr<Object> GetObject(const std::string &name);

        void DelObject(const std::string &name);

    protected:
        static BeanFactoryWrap *sl_pBeanFactoryWrap;
        std::mutex m_Mutex;
        std::unordered_map<std::string, std::shared_ptr<BeanFactoryBase>> m_mapCreateFunction;

        std::mutex m_mutexObject;
        std::unordered_map<std::string, std::shared_ptr<Object>> m_mapObject;
    };

    template<typename ...Targs>
    class BeanFactory : public BeanFactoryBase {
    public:
        BeanFactory() {};

        virtual ~BeanFactory(){};

        bool Regist(const std::string& strCreateTypeName, const std::string& strTypeName, std::function<Object*(Targs&&... args)> pFunc);
        bool UnRegist(const std::string& strCreateTypeName, const std::string& strTypeName);
        Object* Create(const std::string& strCreateTypeName, const std::string& strTypeName, Targs&&... args);
        void GetRegistBeans(std::vector<std::string> &dst);


        std::string TypeID() override;

    private:
        std::mutex m_Mutex;
        std::unordered_map<std::string, std::function<Object*(Targs&&...)> > m_mapCreateFunction;
    };

    template<typename ...Targs>
    bool BeanFactory<Targs...>::Regist(const std::string& strCreateTypeName, const std::string& strTypeName, std::function<Object*(Targs&&... args)> pFunc)
    {
        if (m_pHook) {
            m_pHook(this, "Regist", strCreateTypeName, strTypeName);
        }
        if (nullptr == pFunc)
        {
            return (false);
        }
        std::lock_guard<std::mutex> lock(m_Mutex);
        bool bReg = m_mapCreateFunction.insert(
                std::make_pair(strTypeName, pFunc)).second;
        return (bReg);
    }

    template<typename ...Targs>
    bool BeanFactory<Targs...>::UnRegist(const std::string& strCreateTypeName, const std::string& strTypeName)
    {
        if (m_pHook) {
            m_pHook(this, "UnRegist", strCreateTypeName, strTypeName);
        }
        std::lock_guard<std::mutex> lock(m_Mutex);
        auto iter = m_mapCreateFunction.find(strTypeName);
        if (iter == m_mapCreateFunction.end())
        {
            return (false);
        }
        else
        {
            m_mapCreateFunction.erase(iter);
            return (true);
        }
    }

    template<typename ...Targs>
    Object* BeanFactory<Targs...>::Create(const std::string& strCreateTypeName, const std::string& strTypeName, Targs&&... args)
    {
        if (m_pHook) {
            m_pHook(this, "Create", strCreateTypeName, strTypeName);
        }
        std::lock_guard<std::mutex> lock(m_Mutex);
        auto iter = m_mapCreateFunction.find(strTypeName);
        if (iter == m_mapCreateFunction.end())
        {
            fprintf(stderr, "%s %d %s no CreateObject found for \"%s\"",  __FILE__, __LINE__, __FUNCTION__,  strTypeName.c_str());
            return (nullptr);
        }
        else
        {
            return (iter->second(std::forward<Targs>(args)...));
        }
    }

    template<typename... Targs>
    void BeanFactory<Targs...>::GetRegistBeans(std::vector<std::string> &dst) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        for (auto &item : m_mapCreateFunction) {
            dst.template emplace_back(item.first);
        }
    }

    template<typename... Targs>
    std::string BeanFactory<Targs...>::TypeID() {
        return typeid(BeanFactory<Targs...>).name();
    }

}

#endif //OFDB_BEAN_FACTORY_H
