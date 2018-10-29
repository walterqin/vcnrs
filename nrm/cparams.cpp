#include "cparams.h"

CParams* CParams::instance_ = nullptr;

CParams* CParams::instance() {
    if (!instance_) {
        instance_ = new CParams;
    }
    return instance_;
}
