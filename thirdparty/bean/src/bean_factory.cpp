//
// Created by 叶盛 on 2022/10/25.
//
#include <bean/include/bean_factory.h>

namespace bean {
    std::function<void(BeanFactoryBase*, const std::string &, const std::string &, const std::string &)> BeanFactoryBase::m_pHook = nullptr;
    BeanFactoryWrap *BeanFactoryWrap::sl_pBeanFactoryWrap = nullptr;
    BeanFactoryWrap *bean::BeanFactoryWrap::Instance() {
        if (sl_pBeanFactoryWrap == nullptr) {
            sl_pBeanFactoryWrap = new BeanFactoryWrap();
        }
        return sl_pBeanFactoryWrap;
    }

    void BeanFactoryWrap::SetObject(const std::string &name, std::shared_ptr<Object> newObj) {
        std::lock_guard<std::mutex> lock(m_mutexObject);
        m_mapObject[name] = std::move(newObj);
    }

    std::shared_ptr<Object> BeanFactoryWrap::GetObject(const std::string &name) {
        std::lock_guard<std::mutex> lock(m_mutexObject);
        std::shared_ptr<Object> ret;
        if (m_mapObject.count(name)) {
            ret = m_mapObject[name];
        }
        return ret;
    }

    void BeanFactoryWrap::DelObject(const std::string &name) {
        std::lock_guard<std::mutex> lock(m_mutexObject);
        m_mapObject.erase(name);
    }
}


