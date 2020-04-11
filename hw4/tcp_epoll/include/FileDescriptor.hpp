#ifndef FILEDESCRIPTOR_HPP
#define FILEDESCRIPTOR_HPP


namespace Tcp_epoll {

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

} // namespace Tcp_epoll


#endif // FILEDESCRIPTOR_HPP
