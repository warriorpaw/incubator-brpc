// Copyright (c) 2016 baidu-rpc authors.
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef BRPC_POLICY_ESP_AUTHENTICATOR_H
#define BRPC_POLICY_ESP_AUTHENTICATOR_H

#include "brpc/authenticator.h"


namespace brpc {
namespace policy {

class EspAuthenticator: public Authenticator {
public:
    int GenerateCredential(std::string* auth_str) const;

    int VerifyCredential(const std::string& auth_str,
                         const base::EndPoint& client_addr,
                         AuthContext* out_ctx) const;
};

const Authenticator* global_esp_authenticator();

}  // namespace policy
} // namespace brpc


#endif // BRPC_POLICY_GIANO_AUTHENTICATOR_H
