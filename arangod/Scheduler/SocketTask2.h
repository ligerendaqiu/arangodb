////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2014-2016 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
/// @author Achim Brandt
////////////////////////////////////////////////////////////////////////////////

#ifndef ARANGOD_SCHEDULER_SOCKET_TASK2_H
#define ARANGOD_SCHEDULER_SOCKET_TASK2_H 1

#include "Scheduler/Task2.h"

#include "Basics/Thread.h"
#include "Statistics/StatisticsAgent.h"

#ifdef _WIN32
#include "Basics/win-utils.h"
#endif

#include "Basics/socket-utils.h"

namespace arangodb {
namespace basics {
class StringBuffer;
}

namespace rest {

class SocketTask2 : virtual public Task2, public ConnectionStatisticsAgent {
 private:
  explicit SocketTask2(SocketTask2 const&);
  SocketTask2& operator=(SocketTask2 const&);

 private:
  static size_t const READ_BLOCK_SIZE = 10000;

 public:
  SocketTask2(EventLoop2, TRI_socket_t, double timeout);
  ~SocketTask2() {}

#if 0
  //////////////////////////////////////////////////////////////////////////////
  /// set a request timeout
  //////////////////////////////////////////////////////////////////////////////

 public:
  void setKeepAliveTimeout(double);

 protected:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief fills the read buffer
  ///
  /// The function should be called by the input task if the scheduler has
  /// indicated that new data is available. It will return true, if data could
  /// be read and false if the connection has been closed.
  //////////////////////////////////////////////////////////////////////////////

  virtual bool fillReadBuffer();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief handles a read
  //////////////////////////////////////////////////////////////////////////////

  virtual bool handleRead() = 0;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief handles a write
  //////////////////////////////////////////////////////////////////////////////

  virtual bool handleWrite();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief called if write buffer has been sent
  ///
  /// This called is called if the current write buffer has been sent
  /// completly to the client.
  //////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////
  /// @brief handles a keep-alive timeout
  //////////////////////////////////////////////////////////////////////////////

  virtual void handleTimeout() = 0;

 protected:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief sets an active write buffer
  //////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////
  /// @brief checks for presence of an active write buffer
  //////////////////////////////////////////////////////////////////////////////

 protected:
  bool setup(Scheduler*, EventLoop) override;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief cleans up the task by unregistering all watchers
  //////////////////////////////////////////////////////////////////////////////

  void cleanup() override;

  bool handleEvent(EventToken token, EventType) override;

 protected:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief event for keep-alive timeout
  //////////////////////////////////////////////////////////////////////////////

  EventToken _keepAliveWatcher;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief event for read
  //////////////////////////////////////////////////////////////////////////////

  EventToken _readWatcher;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief event for write
  //////////////////////////////////////////////////////////////////////////////

  EventToken _writeWatcher;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief communication socket
  //////////////////////////////////////////////////////////////////////////////

  TRI_socket_t _commSocket;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief keep-alive timeout in seconds
  //////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////
  /// @brief the current write buffer
  //////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////
  /// @brief the current write buffer statistics
  //////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////
  /// @brief number of bytes already written
  //////////////////////////////////////////////////////////////////////////////

  size_t _writeLength;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief read buffer
  ///
  /// The function fillReadBuffer stores the data in this buffer.
  //////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////
  /// @brief client has closed the connection
  //////////////////////////////////////////////////////////////////////////////

  bool _clientClosed;

 private:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief current thread identifier
  //////////////////////////////////////////////////////////////////////////////

  TRI_tid_t _tid;
#endif

 public:
  void start();

 protected:
  virtual bool processRead() = 0;

 protected:
  void addWriteBuffer(std::unique_ptr<basics::StringBuffer> buffer) {
    addWriteBuffer(std::move(buffer), (RequestStatisticsAgent*)nullptr);
  }

  void addWriteBuffer(std::unique_ptr<basics::StringBuffer>,
                      RequestStatisticsAgent*);

  void addWriteBuffer(basics::StringBuffer*, TRI_request_statistics_t*);

  void completedWriteBuffer();

  void closeStream();

 protected:
  ConnectionInfo _connectionInfo;

  basics::StringBuffer* _readBuffer = nullptr;

  basics::StringBuffer* _writeBuffer = nullptr;
  TRI_request_statistics_t* _writeBufferStatistics = nullptr;

  std::deque<basics::StringBuffer*> _writeBuffers;
  std::deque<TRI_request_statistics_t*> _writeBuffersStats;

  boost::asio::ip::tcp::socket _stream;

 protected:
  bool _closeRequested = false;

 private:
  bool reserveMemory();
  bool trySyncRead();
  void asyncReadSome();
  void closeReceiveStream();

 private:
  bool _closedSend = false;
  bool _closedReceive = false;
};
}
}

#endif
