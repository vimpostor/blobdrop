#pragma once

namespace Version {

#ifndef BLOBDROP_VERSION
// fallback version, if cmake failed to set the version
#define BLOBDROP_VERSION "1.0"
#endif

const char *version_string();

}
