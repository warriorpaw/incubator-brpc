// Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
// File test_baidu_naming_service.cpp
// Author: The baidu-rpc authors (pbrpc@baidu.com)
// Date 2014/10/20 13:50:10

#include <stdio.h>
#include <gtest/gtest.h>
#include <vector>
#include "base/string_printf.h"
#include "base/files/temp_file.h"
#include "brpc/socket.h"
#include "brpc/channel.h"
#include "brpc/load_balancer.h"
#include "brpc/policy/file_naming_service.h"

class NamingServiceFilterTest : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
}; 

class MyNSFilter: public brpc::NamingServiceFilter {
public:
    bool Accept(const brpc::ServerNode& node) const {
        return node.tag == "enable";
    }
};

TEST_F(NamingServiceFilterTest, sanity) {
    std::vector<brpc::ServerNode> servers;
    const char *address_list[] =  {
        "10.127.0.1:1234",
        "10.128.0.1:1234 enable",
        "10.129.0.1:1234",
        "localhost:1234",
        "brpc.baidu.com:1234"
    };
    base::TempFile tmp_file;
    {
        FILE* fp = fopen(tmp_file.fname(), "w");
        for (size_t i = 0; i < ARRAY_SIZE(address_list); ++i) {
            ASSERT_TRUE(fprintf(fp, "%s\n", address_list[i]));
        }
        fclose(fp);
    }
    MyNSFilter ns_filter;
    brpc::Channel channel;
    brpc::ChannelOptions opt;
    opt.ns_filter = &ns_filter;
    std::string ns = std::string("file://") + tmp_file.fname();
    ASSERT_EQ(0, channel.Init(ns.c_str(), "rr", &opt));

    base::EndPoint ep;
    ASSERT_EQ(0, base::hostname2endpoint("10.128.0.1:1234", &ep));
    for (int i = 0; i < 10; ++i) {
        brpc::SocketUniquePtr tmp_sock;
        brpc::LoadBalancer::SelectIn sel_in = { 0, false, 0, NULL };
        brpc::LoadBalancer::SelectOut sel_out(&tmp_sock);
        ASSERT_EQ(0, channel._lb->SelectServer(sel_in, &sel_out));
        ASSERT_EQ(ep, tmp_sock->remote_side());
    }
}
