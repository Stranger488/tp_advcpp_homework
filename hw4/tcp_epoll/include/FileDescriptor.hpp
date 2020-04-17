#ifndef FILEDESCRIPTOR_HPP
#define FILEDESCRIPTOR_HPP


namespace tcp_epoll {

class FileDescriptor {
public:
    FileDescriptor() = default;
    FileDescriptor(int fd);
    FileDescriptor(FileDescriptor&& other);
    ~FileDescriptor() noexcept;

    FileDescriptor& operator=(int fd);

    FileDescriptor(const FileDescriptor& fd) = delete;

    operator int() const;
    int extract();
    void close();

private:
    int fd_ = -1;
};

} // namespace tcp_epoll


#endif // FILEDESCRIPTOR_HPP
