//
// Created by 叶盛 on 2022/10/25.
//

#ifndef OFDB_BEAN_OBJECT_H
#define OFDB_BEAN_OBJECT_H

#pragma once

#include "status.h"

namespace bean {
    class Object {
    public:
        Object() {}
        virtual ~Object() {}
        virtual std::string TypeID () {
            return typeid(Object).name();
        }
    };

    class Client : public Object {
    public:
        explicit Client() {}

        virtual ~Client() {}

        virtual std::string TypeID () {
            return typeid(Client).name();
        }

        // No copying allowed
        Client(const Client&) = delete;
        Client& operator=(const Client&) = delete;

        virtual Status Init(const std::string &server_conf_path) = 0;

        virtual Status Run() = 0;

        virtual void Exit() = 0;
    };

    class Server : public Object {
    public:
        explicit Server() {}

        virtual ~Server() {}

        virtual std::string TypeID () {
            return typeid(Server).name();
        }

        // No copying allowed
        Server(const Server&) = delete;
        Server& operator=(const Server&) = delete;

        virtual Status Init(const std::string &server_conf_path) = 0;

        virtual Status Start() = 0;

        virtual Status Stop() = 0;

        virtual void Exit() = 0;
    };
}

#endif //OFDB_BEAN_OBJECT_H
