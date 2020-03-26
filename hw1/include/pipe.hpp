#ifndef PIPE_HPP
#define PIPE_HPP


class Pipe {
public:
    Pipe();
    ~Pipe() noexcept;

    int get_pipe_in();
    int get_pipe_out();

    int extract_in();
    int extract_out();

    void close_in();
    void close_out();

private:
    int fds_[2];
};


#endif // PIPE_HPP
