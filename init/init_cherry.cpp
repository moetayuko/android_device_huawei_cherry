/*
   Copyright (C) 2017-2018 The Android Open Source Project

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <android-base/logging.h>
#include <cstdlib>
#include <fcntl.h>
#include <fstream>
#include <string>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <unistd.h>

#include "property_service.h"
#include "log.h"

const char *APP_INFO = "/proc/app_info";

using namespace std;

namespace android {
namespace init {

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void set_model(const char *model) {
    property_override("ro.build.product", model);
    property_override("ro.product.device", model);
    property_override("ro.product.model", model);
    property_override("ro.vendor.product.device", model);
}

void vendor_load_properties() {
    ifstream fin;
    string buf;

    fin.open(APP_INFO, ios::in);
    if (!fin) {
        LOG(ERROR) << __func__ << ": Failed to open " << APP_INFO;
        return;
    }

    while (getline(fin, buf))
        if (buf.find("huawei_fac_product_name") != string::npos)
            break;
    fin.close();

    /* C8817D */
    if (buf.find("C8817D") != string::npos) {
        set_model("C8817D");
    }
    /* C8817E */
    else if (buf.find("C8817E") != string::npos) {
        set_model("C8817E");
    }
    /* G621-TL00 */
    else if (buf.find("G621-TL00") != string::npos) {
        set_model("G621-TL00");
    }
    /* G620S-UL00 */
    else if (buf.find("G620S-UL") != string::npos) {
        set_model("G620S-UL00");
    }
    /* G620S-L01 */
    else if (buf.find("G620S-L01") != string::npos) {
        set_model("G620S-L01");
    }
    /* G620S-L02 */
    else if (buf.find("G620S-L02") != string::npos) {
        set_model("G620S-L02");
    }
    /* Che1-CL10 */
    else if (buf.find("Che1-CL10") != string::npos) {
        set_model("Che1-CL10");
    }
    /* Che1-CL20 */
    else if (buf.find("Che1-CL20") != string::npos) {
        set_model("Che1-CL20");
    }
    /* Che1-L04 */
    else if (buf.find("Che1-L04") != string::npos) {
        set_model("Che1-L04");
    }
    else {
        LOG(ERROR) << __func__ << ": unexcepted huawei_fac_product_name!";
    }
}

}  // namespace init
}  // namespace android
