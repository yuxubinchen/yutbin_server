#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <mutex>
#include "Connector.hpp"
namespace yb
{
    namespace net
    {   class Connector;
        using ConnectorPtr=std::shared_ptr<Connector>;
        class TcpClient : nocopy
        {
        public:
            using TcpConnectionPtr=std::shared_ptr<TcpConnection>; 
            TcpClient(Eventloop *loop,
                      const InetAddress &serverAddr,
                      const std::string &nameArg);
            ~TcpClient();
            void connect();
            void disconnect();
            void stop();
            TcpConnectionPtr connection()
            {
                std::lock_guard<std::mutex> lock(mutex_);
                return connection_;
            }
            Eventloop *getLoop() const { return loop_; }
            bool retry() const { return retry_; }
            void enableRetry() { retry_ = true; }

            const std::string &name() const
            {
                return name_;
            }

            /// Set connection callback.
            /// Not thread safe.
            void setConnectionCallback(ConnectionCallback cb)
            {
                connectionCallback_ = std::move(cb);
            }

            /// Set message callback.
            /// Not thread safe.
            void setMessageCallback(MessageCallback cb)
            {
                messageCallback_ = std::move(cb);
            }

            /// Set write complete callback.
            /// Not thread safe.
            void setWriteCompleteCallback(WriteCompleteCallback cb)
            {
                writeCompleteCallback_ = std::move(cb);
            }

        private:
            /// Not thread safe, but in loop
            void newConnection(int sockfd);
            /// Not thread safe, but in loop
            void removeConnection(const TcpConnectionPtr &conn);

            Eventloop *loop_;
            ConnectorPtr connector_; // avoid revealing Connector
            const std::string name_;
            ConnectionCallback connectionCallback_;
            MessageCallback messageCallback_;
            WriteCompleteCallback writeCompleteCallback_;
            bool retry_;   // atomic
            bool connect_; // atomic
            // always in loop thread
            int nextConnId_;
            mutable std::mutex mutex_;
            TcpConnectionPtr connection_;
        };
    }
}
#endif