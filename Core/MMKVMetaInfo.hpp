/*
 * Tencent is pleased to support the open source community by making
 * MMKV available.
 *
 * Copyright (C) 2018 THL A29 Limited, a Tencent company.
 * All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use
 * this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 *       https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MMKV_MMKVMETAINFO_H
#define MMKV_MMKVMETAINFO_H
#ifdef __cplusplus

#include "aes/AESCrypt.h"
#include <cstdint>
#include <cstring>

namespace mmkv {

enum MMKVVersion : uint32_t {
    MMKVVersionDefault = 0,

    // record full write back count 记录完整的写回次数
    MMKVVersionSequence = 1,

    // store random iv for encryption 存储用于加密的随机iv
    MMKVVersionRandomIV = 2,

    // store actual size together with crc checksum, try to reduce file corruption 实际大小和crc校验一起存储，尽量减少文件损坏
    MMKVVersionActualSize = 3,
};

struct MMKVMetaInfo {
    uint32_t m_crcDigest = 0;
    uint32_t m_version = MMKVVersionSequence;
    uint32_t m_sequence = 0; // full write-back count
    uint8_t m_vector[AES_KEY_LEN] = {}; // uint8_t _t意味着别名，该类型为unsigned char
    uint32_t m_actualSize = 0;

    // confirmed info: it's been synced to file
    struct {
        uint32_t lastActualSize = 0;
        uint32_t lastCRCDigest = 0;
        uint32_t _reserved[16] = {};
    } m_lastConfirmedMetaInfo;

    void write(void *ptr) const {
        MMKV_ASSERT(ptr);
        memcpy(ptr, this, sizeof(MMKVMetaInfo));
    }

    void writeCRCAndActualSizeOnly(void *ptr) const {
        MMKV_ASSERT(ptr);
        auto other = (MMKVMetaInfo *) ptr;
        other->m_crcDigest = m_crcDigest;
        other->m_actualSize = m_actualSize;
    }

    void read(const void *ptr) {
        MMKV_ASSERT(ptr);
        memcpy(this, ptr, sizeof(MMKVMetaInfo));
    }
};

static_assert(sizeof(MMKVMetaInfo) <= (4 * 1024), "MMKVMetaInfo lager than one pagesize");

} // namespace mmkv

#endif
#endif //MMKV_MMKVMETAINFO_H
