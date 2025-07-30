#include "filemanager/impl/LocalOutputStream.h"

namespace milvus {

LocalOutputStream::LocalOutputStream(const std::string& filename) : filename_(filename) {
    stream_.open(filename_, std::ios::binary | std::ios::out);
    if (!stream_.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename_);
    }
}

LocalOutputStream::~LocalOutputStream() {
    stream_.close();
}

size_t
LocalOutputStream::Tell() const {
    return stream_.tellp();
}

size_t
LocalOutputStream::Write(const void* ptr, size_t size) {
    stream_.write(static_cast<const char*>(ptr), size);
    return size;
}

}  // namespace milvus
