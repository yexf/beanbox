//
// Created by 叶盛 on 2022/10/26.
//

#include "bbox_conf.h"
#include "bbox_define.h"
BBoxConf::BBoxConf(const std::string &path) : BaseConf(path) {
    pthread_rwlock_init(&rwlock_, NULL);
}

BBoxConf::~BBoxConf() {
    pthread_rwlock_destroy(&rwlock_);
}

int BBoxConf::Load() {
    int ret = LoadConf();
    if (ret != 0) {
        return ret;
    }

    GetConfStr("server-id", &server_id_);
    if (server_id_.empty()) {
        server_id_ = "1";
    } else if (BBOX_SERVER_ID_MAX < std::stoull(server_id_)) {
        server_id_ = "BBOX_SERVER_ID_MAX";
    }
    GetConfStr("log-path", &log_path_);
    log_path_ = log_path_.empty() ? "./log/" : log_path_;
    if (log_path_[log_path_.length() - 1] != '/') {
        log_path_ += "/";
    }
    GetConfStr("pidfile", &pidfile_);

    // daemonize
    std::string dmz;
    GetConfStr("daemonize", &dmz);
    daemonize_ =  (dmz == "yes") ? true : false;

    GetConfStr("box-start", &box_start_);
    GetConfStr("box-name", &box_name_);
    GetConfStr("box-type", &box_type_);
    GetConfStr("box-conf", &box_conf_);

    return ret;
}

int BBoxConf::ConfigRewrite() {
    ReloadConf();
    RWLock l(&rwlock_, true);
    // Only set value for config item that can be config set.
    SetConfStr("box-start", box_start_);
    SetConfStr("box-name", box_name_);
    SetConfStr("box-type", box_type_);
    SetConfStr("box-conf", box_conf_);

    return WriteBack();
}

