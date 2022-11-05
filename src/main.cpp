//
// Created by 叶盛 on 2022/10/25.
//

#include <signal.h>
#include <glog/logging.h>
#include <cstdlib>
#include <fcntl.h>

#include <bbox_version.h>
#include <slash/include/env.h>

#include <algorithm>
#include <bean/include/proctitle_helper.h>
#include "bean/include/bean.h"
#include "bbox_conf.h"
#include "bbox_define.h"

std::shared_ptr<bean::Server> g_bbox_server;
std::shared_ptr<bean::Client> g_bbox_client;
BBoxConf *g_bbox_conf = nullptr;
static void version() {
    char version[32];
    snprintf(version, sizeof(version), "%d.%d.%d", BEAN_BOX_MAJOR,
             BEAN_BOX_MINOR, BEAN_BOX_PATCH);
    printf("-----------bean box %s ----------\n", version);
}

static void usage()
{
    char version[32];
    snprintf(version, sizeof(version), "%d.%d.%d", BEAN_BOX_MAJOR,
             BEAN_BOX_MINOR, BEAN_BOX_PATCH);
    fprintf(stderr,
            "bean box module %s\n"
            "usage: bbox [-hv] [-c conf/file] [-s stop]\n"
            "\t-h               -- show this help\n"
            "\t-c conf/file     -- config file \n"
            "\t-s stop          -- stop pid \n"
            "  example: ./bin/bbox -c ./conf/bbox.conf\n",
            version
    );
}

static void create_pid_file(void) {
    /* Try to write the pid file in a best-effort way. */
    std::string path(g_bbox_conf->pidfile());

    size_t pos = path.find_last_of('/');
    if (pos != std::string::npos) {
        // mkpath(path.substr(0, pos).c_str(), 0755);
        slash::CreateDir(path.substr(0, pos));
    } else {
        path = kBBoxPidFile;
    }

    FILE *fp = fopen(path.c_str(), "w");
    if (fp) {
        fprintf(fp,"%d\n",(int)getpid());
        fclose(fp);
    }
}

static int get_pid_file(void) {
    /* Try to write the pid file in a best-effort way. */
    std::string path(g_bbox_conf->pidfile());
    int pid = -1;
    FILE *fp = fopen(path.c_str(), "r");
    if (fp) {
        fscanf(fp,"%d\n", &pid);
        fclose(fp);
    }
    return pid;
}

static void close_std() {
    int fd;
    if ((fd = open("/dev/null", O_RDWR, 0)) != -1) {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        close(fd);
    }
}

static void BeanBoxConfInit(const std::string& path) {
    printf("path : %s\n", path.c_str());
    g_bbox_conf = new BBoxConf(path);
    if (g_bbox_conf->Load() != 0) {
        LOG(FATAL) << "bean box load conf error";
    }
    version();
    printf("-----------bean box config list----------\n");
    g_bbox_conf->DumpConf();
    printf("-----------bean box config end----------\n");
}

static void BeanBoxGlogInit() {
    if (!slash::FileExists(g_bbox_conf->log_path())) {
        slash::CreatePath(g_bbox_conf->log_path());
    }

    if (!g_bbox_conf->daemonize()) {
        FLAGS_alsologtostderr = true;
    }
    FLAGS_log_dir = g_bbox_conf->log_path();
    FLAGS_minloglevel = 0;
    FLAGS_max_log_size = 1800;
    FLAGS_logbufsecs = 0;
    ::google::InitGoogleLogging("bbox");
}

static void IntSigHandle(const int sig) {
    LOG(INFO) << "Catch Signal " << sig << ", cleanup...";
    if (g_bbox_server) {
        g_bbox_server->Exit();
    }
    if (g_bbox_client) {
        g_bbox_client->Exit();
    }
}

static void BeanBoxSignalSetup() {
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, &IntSigHandle);
    signal(SIGQUIT, &IntSigHandle);
    signal(SIGTERM, &IntSigHandle);
}

static void daemonize() {
    if (fork() != 0) exit(0); /* parent exits */
    setsid(); /* create a new session */
}

void test() {
    std::unordered_map<std::string, std::vector<std::string>> bean_list;
    bean::GetRegistBean(bean_list);
    printf("get regist bean :\n");
    for (auto &item : bean_list) {
        for (auto &item2 : item.second) {
            printf("regist bean : %s -> %s\n", item.first.c_str(), item2.c_str());
        }
    }

    std::shared_ptr<bean::Server> pServer = bean::Bean::MakeSharedServer("libofdb.so", "ofdb::OfdbServer");
    if (pServer) {
        pServer->Start();

        pServer->Exit();
    }
    bean_list.clear();
    bean::GetRegistBean(bean_list);
    printf("get regist bean :\n");
    for (auto &item : bean_list) {
        for (auto &item2 : item.second) {
            printf("regist bean : %s -> %s\n", item.first.c_str(), item2.c_str());
        }
    }
}

bean::Status InitBoxStart() {
    bean::Status ret = bean::Status::NotSupported("TODO");
    std::string start = g_bbox_conf->box_start();

    std::vector<std::string> vecStart;
    slash::StringSplit(start, BBOX_START_DELIMITER, vecStart);
    LOG(INFO) << "start " << start << "\n";
    if (vecStart.size() < 2) {
        return bean::Status::NotSupported("Start Error");
    }
    if (g_bbox_conf->box_type() == "server") {
        g_bbox_server = bean::Bean::MakeSharedServer(vecStart[1], vecStart[0]);
        if (!g_bbox_server) {
            return bean::Status::NotFound("Make Shared Service Error");
        }
        ret = g_bbox_server->Init(g_bbox_conf->box_conf());
    }
    else if (g_bbox_conf->box_type() == "client") {
        g_bbox_client = bean::Bean::MakeSharedClient(vecStart[1], vecStart[0]);
        if (!g_bbox_client) {
            return bean::Status::NotFound("Make Shared Client Error");
        }
        ret = g_bbox_client->Init(g_bbox_conf->box_conf());
    }
    std::string name = g_bbox_conf->box_type() + ": " + g_bbox_conf->box_name() + " ==>> " + g_bbox_conf->box_start();
    ngx_setproctitle(name.c_str());
    return ret;
}

void DeInitBox() {
    std::string start = g_bbox_conf->box_start();

    std::vector<std::string> vecStart;
    slash::StringSplit(start, BBOX_START_DELIMITER, vecStart);

    if (vecStart.size() < 2) {
        return;
    }
    if (g_bbox_conf->box_type() == "server") {
        bean::Bean::DeleteSharedServer(vecStart[0]);
    }
    else if (g_bbox_conf->box_type() == "client") {
        bean::Bean::DeleteSharedClient(vecStart[0]);
    }
    LOG(INFO) << "stop " << start;
    usleep(100000);
}

int main(int argc, char *argv[]) {
    ngx_init_setproctitle(argc, argv);
    if (argc != 2 && argc != 3 && argc != 5) {
        usage();
        exit(-1);
    }
    bool path_opt = false;
    bool signal_opt = false;
    char c;
    char path[1024];
    char signal[1024];
    while (-1 != (c = getopt(argc, argv, "c:s:hv"))) {
        switch (c) {
            case 'c':
                snprintf(path, 1024, "%s", optarg);
                path_opt = true;
                break;
            case 's':
                snprintf(signal, 1024, "%s", optarg);
                signal_opt = true;
                break;
            case 'h':
                usage();
                return 0;
            case 'v':
                version();
                return 0;
            default:
                usage();
                return 0;
        }
    }

    if (path_opt == false) {
        fprintf (stderr, "Please specify the conf file path\n" );
        usage();
        exit(-1);
    }

    BeanBoxConfInit(path);

    if (signal_opt == true) {
        int pid = get_pid_file();
        if (pid < 0) {
            fprintf (stderr, "get pid file error\n" );
            exit(-1);
        }
        if (strcmp("stop", signal) == 0) {
            kill(pid, SIGINT);
            fprintf (stdout, "send SIGINT to pid %d\n", pid);
        }
        else {
            fprintf (stderr, "unknow signal %s\n", signal);
            exit(-1);
        }
        return 0;
    }

    // daemonize if needed
    if (g_bbox_conf->daemonize()) {
        daemonize();
        create_pid_file();
    }

    BeanBoxGlogInit();
    BeanBoxSignalSetup();

    LOG(INFO) << "Server at: " << path;
    bean::Status ret = InitBoxStart();

    if (!ret.ok()) {
        LOG(ERROR) << "init server error: " << ret.ToString();
        exit(1);
    }

    if (g_bbox_conf->daemonize()) {
        close_std();
    }

    if (g_bbox_client) {
        g_bbox_client->Run();
    }

    if (g_bbox_server) {
        g_bbox_server->Start();
    }

    if (g_bbox_conf->daemonize()) {
        unlink(g_bbox_conf->pidfile().c_str());
    }

    if (g_bbox_server) {
        g_bbox_server->Stop();
    }

    DeInitBox();
    g_bbox_client = nullptr;
    g_bbox_server = nullptr;
    ::google::ShutdownGoogleLogging();
    delete g_bbox_conf;
    return 0;
}
