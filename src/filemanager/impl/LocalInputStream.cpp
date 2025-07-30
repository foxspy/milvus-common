#include "filemanager/impl/LocalInputStream.h"

namespace milvus {

LocalInputStream::LocalInputStream(const std::string& filename) : filename_(filename) {
    stream_.open(filename_, std::ios::binary | std::ios::in);
    if (!stream_.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename_);
    }

    stream_.seekg(0, std::ios::end);
    size_ = static_cast<size_t>(stream_.tellg());
    stream_.seekg(0, std::ios::beg);
}

LocalInputStream::~LocalInputStream() {
    stream_.close();
}

bool
LocalInputStream::Seek(int64_t offset) {
    stream_.seekg(offset);
    return true;
}

size_t
LocalInputStream::Size() const {
    return size_;
}

size_t
LocalInputStream::Tell() const {
    return stream_.tellg();
}

bool
LocalInputStream::Eof() const {
    return stream_.eof();
}

size_t
LocalInputStream::Read(void* ptr, size_t size) {
    size_t cur = stream_.tellg();
    if (cur + size > size_) {
        throw std::runtime_error("Read out of range");
    }
    stream_.read(static_cast<char*>(ptr), size);
    return stream_.gcount();
}
}  // namespace milvus
