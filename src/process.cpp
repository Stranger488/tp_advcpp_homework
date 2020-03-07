#include "../include/process.h"

Process::Process(const std::string &path) {

}

Process::~Process() {

}

size_t Process::write(const void *data, size_t len) {
    return 0;
}

void Process::writeExact(const void *data, size_t len) {

}

size_t Process::read(void *data, size_t len) {
    return 0;
}

void Process::readExact(void *data, size_t len) {

}

bool Process::isReadable() const {
    return false;
}

void Process::closeStdin() {

}

void Process::close() {

}
