/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2015 Scientific Computing and Imaging Institute,
   University of Utah.


   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/
/// @todo Documentation Core/Logging/Log.h

#ifndef CORE_LOGGING_LOG_H
#define CORE_LOGGING_LOG_H

#include <string>
#include <Core/Logging/LoggerFwd.h>
#ifndef Q_MOC_RUN
#include <Core/Utils/Singleton.h>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/path.hpp>
#include <spdlog/spdlog.h>
#endif
#include <Core/Logging/share.h>

namespace SCIRun
{
  namespace Core
  {
    namespace Logging
    {
      //Copied from http://jonblack.org/2013/01/27/wrapping-a-logging-framework-in-cpp/

      /// Logging levels used by pix. Follows the same as for syslog, taken from
      /// RFC 5424. Comments added for ease of reading.
      /// @see http://en.wikipedia.org/wiki/Syslog.
      enum LogLevel
      {
        EMERG,  // System is unusable (e.g. multiple parts down)
        ALERT,  // System is unusable (e.g. single part down)
        CRIT,   // Failure in non-primary system (e.g. backup site down)
        ERROR_LOG,  // Non-urgent failures; relay to developers
        WARN,   // Not an error, but indicates error will occur if nothing done.
        NOTICE, // Events that are unusual, but not error conditions.
        INFO,   // Normal operational messages. No action required.
        DEBUG_LOG,  // Information useful during development for debugging.
        NOTSET
      };

      class SCISHARE LogAppenderStrategy
      {
      public:
        virtual ~LogAppenderStrategy() {}
        virtual void log4(const std::string& message) const = 0;
      };

      typedef boost::shared_ptr<LogAppenderStrategy> LogAppenderStrategyPtr;

      class SCISHARE Log1 final
      {
      public:
        static Log1& get();
        static Log1& get(const std::string& name);

        static void setLogDirectory(const boost::filesystem::path& dir);
        static boost::filesystem::path logDirectory();

        class SCISHARE Stream
        {
        public:
          explicit Stream(class LogStreamImpl* impl);
          SCISHARE friend Stream& operator<<(Stream& log, const std::string& msg);
          void stream(const std::string& msg);
          void stream(double x);
          void flush();
        private:
          boost::shared_ptr<class LogStreamImpl> impl_;
        };

        void log(LogLevel level, const std::string& msg);

        SCISHARE friend Stream& operator<<(Log1& log, LogLevel level);

        void setVerbose(bool v);
        bool verbose() const;
        void flush();
        void addCustomAppender(LogAppenderStrategyPtr appender);
        void clearAppenders();

      private:
        Log1();
        explicit Log1(const std::string& name);
        Log1(const Log1&) = delete;
        Log1(Log1&&) = delete;
        Log1& operator=(const Log1&) = delete;
        Log1& operator=(Log1&&) = delete;

        static boost::filesystem::path directory_;
      private:
        friend class Stream;
        boost::shared_ptr<class LogImpl> impl_;
        void init();
      };

      SCISHARE Log1::Stream& operator<<(Log1& log, LogLevel level);
      SCISHARE Log1::Stream& operator<<(Log1::Stream& log, const std::string& msg);
      SCISHARE Log1::Stream& operator<<(Log1::Stream& log, double x);

      template <typename T>
      Log1::Stream& operator<<(Log1::Stream& log, const T& t)
      {
        return log << boost::lexical_cast<std::string>(t);
      }

      SCISHARE Log1::Stream& operator<<(Log1::Stream& log, std::ostream&(*func)(std::ostream&));

      class SCISHARE ApplicationHelper
      {
      public:
        ApplicationHelper();
        boost::filesystem::path configDirectory();
        std::string applicationName();
        bool get_user_directory( boost::filesystem::path& user_dir, bool config_path);
        bool get_config_directory( boost::filesystem::path& config_dir );
        bool get_user_name( std::string& user_name );
      };

      using Logger2 = std::shared_ptr<spdlog::logger>;

      class SCISHARE Log2 
      {
      public:
        explicit Log2(const std::string& name) : name_(name) {}
        Logger2 get();
        void addCustomSink(LogAppenderStrategyPtr appender);
      private:
        std::string name_;
        std::vector<LogAppenderStrategyPtr> appenders_;
      };

      class SCISHARE ModuleLog final : public Log2
      {
        CORE_SINGLETON(ModuleLog)
      public:
        ModuleLog() : Log2("module") {}
      };

      class SCISHARE GeneralLog final: public Log2
      {
        CORE_SINGLETON(GeneralLog)
      public:
        GeneralLog() : Log2("root") {}
      };

      template <class... T>
      void LOG_DEBUG(const char* fmt, T&&... args)
      {
        SCIRun::Core::Logging::GeneralLog::Instance().get()->debug(fmt, args...);
      }

      inline void LOG_DEBUG(const std::string& str)
      {
        SCIRun::Core::Logging::GeneralLog::Instance().get()->debug(str);
      }
    }
  }
}

#define LOG_DEBUG_OLD_MACRO(str) SCIRun::Core::Logging::GeneralLog::Instance().get()->debug(str);

//#define LOG_DEBUG_TO(log, str) log << SCIRun::Core::Logging::DEBUG_LOG << str << std::endl

#endif
