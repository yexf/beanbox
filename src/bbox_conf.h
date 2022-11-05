//
// Created by 叶盛 on 2022/10/26.
//

#ifndef OFDB_BBOX_CONF_H
#define OFDB_BBOX_CONF_H
#include <map>
#include <set>
#include <unordered_set>
#include <atomic>

#include "slash/include/base_conf.h"
#include "slash/include/slash_mutex.h"
#include "slash/include/slash_string.h"

typedef slash::RWLock RWLock;

// global class, class members well initialized
class BBoxConf : public slash::BaseConf {
public:
    BBoxConf(const std::string& path);
    ~BBoxConf();

    // Getter
    std::string server_id()                           { RWLock l(&rwlock_, false); return server_id_; }
    std::string log_level()                           { RWLock l(&rwlock_, false); return log_level_; }

    // Immutable config items, we don't use lock.
    std::string box_start()                           { RWLock l(&rwlock_, false); return box_start_;}
    std::string box_name()                            { RWLock l(&rwlock_, false); return box_name_;}
    std::string box_type()                            { RWLock l(&rwlock_, false); return box_type_;}
    std::string box_conf()                            { RWLock l(&rwlock_, false); return box_conf_;}

    std::string log_path()                            { RWLock l(&rwlock_, false); return log_path_; }
    bool daemonize()                                  { return daemonize_; }
    std::string pidfile()                             { return pidfile_; }

    // Setter
    void SetLogLevel(const std::string &log_level) {
        RWLock l(&rwlock_, true);
        log_level_ = log_level;
    }
    int Load();
    int ConfigRewrite();
private:
    std::string server_id_;
    std::string log_path_;
    std::string log_level_;
    std::string pidfile_;
    bool daemonize_;

    std::string box_start_;
    std::string box_name_;
    std::string box_type_;
    std::string box_conf_;

    //
    // Critical configure items
    //
    pthread_rwlock_t rwlock_;
};

#endif //OFDB_BBOX_CONF_H
