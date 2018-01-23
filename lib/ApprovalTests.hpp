#include <cstring>
#include <exception>
#include <string>
#include <stdio.h>
#include <fstream>
#include <sys/stat.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <numeric>
#include <stack>
#include <sstream>
#include <memory>
 // ******************** From: DiffReporter.h
#ifndef CATCHPLAYGROUND_DIFFREPORTER_H
#define CATCHPLAYGROUND_DIFFREPORTER_H


class DiffReporter : public FirstWorkingReporter
{
public:
    DiffReporter() : FirstWorkingReporter(
            {
                    new Mac::MacDiffReporter(),
                    new Linux::LinuxDiffReporter(),
                    new Windows::WindowsDiffReporter()
            }
    )
    {
    }
};

#endif 

 // ******************** From: ApprovalException.h
#ifndef APPROVALEXCEPTION_H
#define APPROVALEXCEPTION_H


class ApprovalException : public std::exception
{
private:
    char *message;
protected:
    char* init( const char *msg )
    {
        size_t message_sz = std::strlen( msg );
        char *t = new char[message_sz + 1];
        std::strncpy( t, msg, message_sz + 1 );
        return t;
    };
public:
    ApprovalException( const char *msg ) : message( init( msg ) ) {}

    ApprovalException( const ApprovalException &a )
        : message( init( a.message ) )
    {
    }

    virtual ~ApprovalException() throw()
    {
        delete[] message;
    }

    virtual const char *what() const throw()
    {
        return message;
    }
};

class ApprovalMismatchException : public ApprovalException
{
private:
    char *format( const std::string &received, const std::string &approved )
    {
		static const int n = 2048;
        char s[n];
        int size = snprintf( s,
                             n,
                             "Failed Approval: \n"
                                     "Received does not match approved \n"
                                     "Received : \"%s\" \n"
                                     "Approved : \"%s\"",
                             received.c_str(),
                             approved.c_str() );
        char *t = new char[size + 1];
        std::strncpy( t, s, size + 1 );
        return t;
    }
public:
    ApprovalMismatchException( std::string received, std::string approved )
        : ApprovalException( format( received, approved ) )
    {
    }

    ApprovalMismatchException( const ApprovalMismatchException &a )
        : ApprovalException( a ) {}
};

class ApprovalMissingException : public ApprovalException
{
private:
    char *format( const std::string &file )
    {
		static const int n = 1024;
        char s[n];
        int size = snprintf( s,
                             n,
                             "Failed Approval: \n"
                                     "Approval File Not Found \n"
                                     "File: \"%s\"",
                             file.c_str() );
        char *t = new char[size + 1];
        std::strncpy( t, s, size + 1 );
        return t;
    }
public:
    ApprovalMissingException( std::string received, std::string approved )
        : ApprovalException( format( approved ) )
    {
    }

    ApprovalMissingException( const ApprovalMissingException &a )
        : ApprovalException( a )
    {
    }
};

#endif

 // ******************** From: StringWriter.h
#ifndef STRINGWRITER_H
#define STRINGWRITER_H


class StringWriter
{
private:
    std::string s;
    std::string ext;

public:
    StringWriter( std::string contents, std::string fileExtensionWithDot = ".txt" )
        : s( contents ), ext( fileExtensionWithDot ) {}

    std::string GetFileExtension()
    {
        return ext;
    }

    void Write( std::string path )
    {
        std::ofstream out( path.c_str(), std::ofstream::out );
        this->Write( out );
        out.close();
    }

    void Write( std::ostream &out )
    {
        out << s << "\n";
    }


};
#endif

 // ******************** From: FileUtils.h




#ifndef CATCHPLAYGROUND_FILEUTILS_H
#define CATCHPLAYGROUND_FILEUTILS_H


class FileUtils {
public:
    static bool fileExists(std::string path) {
        return fileSize(path) != -1;
    }

    static int fileSize(std::string path) {
        struct stat statbuf;
        int stat_ok = stat(path.c_str(), &statbuf);

        if (stat_ok == -1) {
            return -1;
        }

        return int(statbuf.st_size);
    }

    static void ensureFileExists(std::string fullFilePath) {
        if (!fileExists(fullFilePath)) {
            StringWriter s("", "");
            s.Write(fullFilePath);
        }
    }
};

#endif 

 // ******************** From: SystemUtils.h
#ifndef SYSTEMUTILS_H
#define SYSTEMUTILS_H
// <SingleHpp unalterable>
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
// </SingleHpp>


class SystemUtils
{
public:
    static bool isWindowsOs()
    {
#ifdef _WIN32
        return true;
#else
        return false;
#endif 

    }

    static std::string getDirectorySeparator()
    {
        return isWindowsOs() ? "\\" : "/";
    }


};
#endif

 // ******************** From: DiffInfo.h
#ifndef APPROVALTESTS_CPP_DIFFINFO_H
#define APPROVALTESTS_CPP_DIFFINFO_H


enum class Type { TEXT, IMAGE, TEXT_AND_IMAGE };

class StringUtils
{
public:
    
    static std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); 
        }
        return str;
    }
};

struct DiffInfo
{
    DiffInfo(std::string program, Type type) :
        program(program),
        arguments("%s %s"),
        type(type)
    {
    }
    DiffInfo(std::string program, std::string arguments, Type type) :
        program(program),
        arguments(arguments),
        type(type)
    {
    }
    std::string program;
    std::string arguments;
    Type type;

    std::string getProgramForOs() const
    {
       
        std::string result = program;
        if (result.rfind("{ProgramFiles}", 0) == 0) {
            auto result1 = StringUtils::replaceAll(result, "{ProgramFiles}", "c:\\Program Files\\");
       
            if (FileUtils::fileExists(result1)) {
                result = result1;
            }
            auto result2 = StringUtils::replaceAll(result, "{ProgramFiles}", "c:\\Program Files (x86)\\");
            if (FileUtils::fileExists(result2)) {
                result =  result2;
            }
        }
        return result;
    }
};


#endif 

 // ******************** From: CommandLauncher.h
#ifndef COMMANDLAUNCHER_H
#define COMMANDLAUNCHER_H


class CommandLauncher
{
public:
    virtual ~CommandLauncher() {}
    virtual bool Launch(std::vector<std::string> argv) = 0;
};

class DoNothingLauncher : public CommandLauncher
{
private:
    std::string cmd;
public:
    bool working = true;
    bool Launch(std::vector<std::string> argv)
    {
        for (std::vector<std::string>::iterator it = argv.begin();
            it != argv.end();
            ++it)
        {
            cmd += *it;
            cmd += " ";
        }
        return working;
    }

    std::string ReceivedCommand()
    {
        return cmd;
    }
};

class SystemLauncher : public CommandLauncher
{
public:
    bool exists(const std::string& command)
    {
        bool foundByWhich = false;
        if (!SystemUtils::isWindowsOs()) {
            std::string which = "which " + command + " > /dev/null 2>&1";
            int result = system(which.c_str());
            foundByWhich = (result == 0);
        }
        return  foundByWhich || FileUtils::fileExists(command);

    }

    bool Launch(std::vector<std::string> argv)
    {
        if (!exists(argv.front()))
        {
            return false;
        }

        std::string command = std::accumulate(argv.begin(), argv.end(), std::string(""), [](std::string a, std::string b) {return a + " " + "\"" + b + "\""; });
        std::string launch = SystemUtils::isWindowsOs() ? ("start \"\" " +  command) :  (command + " &");
        system(launch.c_str());
        return true;
}
};

#endif  

 // ******************** From: DiffPrograms.h
#ifndef APPROVALTESTS_CPP_DIFFPROGRAMS_H
#define APPROVALTESTS_CPP_DIFFPROGRAMS_H



#define ENTRY(name, defaultValue) \
        static DiffInfo name() { return defaultValue; }


namespace DiffPrograms {


    namespace Mac {
        ENTRY(DIFF_MERGE,
              DiffInfo("/Applications/DiffMerge.app/Contents/MacOS/DiffMerge", "%s %s -nosplash", Type::TEXT))

        ENTRY(BEYOND_COMPARE, DiffInfo("/Applications/Beyond Compare.app/Contents/MacOS/bcomp", Type::TEXT))

        ENTRY(KALEIDOSCOPE, DiffInfo("/Applications/Kaleidoscope.app/Contents/MacOS/ksdiff", Type::TEXT_AND_IMAGE))

        ENTRY(KDIFF3, DiffInfo("/Applications/kdiff3.app/Contents/MacOS/kdiff3", "%s %s -m", Type::TEXT))

        ENTRY(P4MERGE, DiffInfo("/Applications/p4merge.app/Contents/MacOS/p4merge", Type::TEXT_AND_IMAGE))

        ENTRY(TK_DIFF, DiffInfo("/Applications/TkDiff.app/Contents/MacOS/tkdiff", Type::TEXT))
    }
    namespace Linux {
        
        ENTRY(KDIFF3, DiffInfo("kdiff3", Type::TEXT))

        ENTRY(MELD, DiffInfo("meld", Type::TEXT))
    }
    namespace Windows {
        ENTRY(BEYOND_COMPARE_3, DiffInfo("{ProgramFiles}Beyond Compare 3\\BCompare.exe", Type::TEXT_AND_IMAGE))

        ENTRY(BEYOND_COMPARE_4, DiffInfo("{ProgramFiles}Beyond Compare 4\\BCompare.exe", Type::TEXT_AND_IMAGE))

        ENTRY(TORTOISE_IMAGE_DIFF,
              DiffInfo("{ProgramFiles}TortoiseSVN\\bin\\TortoiseIDiff.exe", "/left:%s /right:%s", Type::IMAGE))

        ENTRY(TORTOISE_TEXT_DIFF, DiffInfo("{ProgramFiles}TortoiseSVN\\bin\\TortoiseMerge.exe", Type::TEXT))

        ENTRY(WIN_MERGE_REPORTER, DiffInfo("{ProgramFiles}WinMerge\\WinMergeU.exe", Type::TEXT))

        ENTRY(ARAXIS_MERGE, DiffInfo("{ProgramFiles}Araxis\\Araxis Merge\\Compare.exe", Type::TEXT))

        ENTRY(CODE_COMPARE, DiffInfo("{ProgramFiles}Devart\\Code Compare\\CodeCompare.exe", Type::TEXT))

        ENTRY(KDIFF3, DiffInfo("{ProgramFiles}KDiff3\\kdiff3.exe", Type::TEXT))
    }
}

#endif 

 // ******************** From: Macros.h
#ifndef CATCHPLAYGROUND_MARCOS_H
#define CATCHPLAYGROUND_MARCOS_H


#define STATIC(type, name, defaultValue) \
      static type &name(type *value = NULL) { \
static type *staticValue; \
if (value != NULL) { \
staticValue = value; \
} \
if (staticValue == NULL) \
{ \
 staticValue = defaultValue; \
} \
return *staticValue; \
} \



#endif 

 // ******************** From: ApprovalNamer.h
#ifndef APPROVALNAMER_H
#define APPROVALNAMER_H


using std::string;

class TestName {
public:
    string fileName;
    string testCase;
    std::vector<string> sections;
};

class ApprovalNamer {
private:
public:
    ApprovalNamer() {
    }

    string getTestName() {
        std::stringstream ext;
        auto test = currentTest();
        for (int i = 0; i < test.sections.size(); i++) {
            if (0 < i) {
                ext << ".";
            }
            ext << test.sections[i];

        }

        return ext.str();
    }

    string getFileName() {
        auto file = currentTest().fileName;

        auto start = file.rfind(SystemUtils::getDirectorySeparator()) + 1;
        auto end = file.rfind(".");
        return file.substr(start, end - start);
    }

    string getDirectory() {
        auto file = currentTest().fileName;
        auto end = file.rfind(SystemUtils::getDirectorySeparator()) + 1;
        return file.substr(0, end);
    }

    STATIC(TestName, currentTest, NULL)

    string getApprovedFile(string extentionWithDot) {

        return getFullFileName(".approved", extentionWithDot);
    }

    string getReceivedFile(string extentionWithDot) {

        return getFullFileName(".received", extentionWithDot);
    }

    string getFullFileName(string approved, string extentionWithDot) {
        std::stringstream ext;
        ext << getDirectory() << getFileName() << "." << getTestName() << approved << extentionWithDot;
        return ext.str();
    }
};

#endif

 // ******************** From: Reporter.h
#ifndef REPORTER_H
#define REPORTER_H


class Reporter {
public:
    virtual bool Report(std::string received, std::string approved) const = 0;
};

class CommandReporter : public Reporter {
private:
    std::string cmd;
    CommandLauncher *l;

protected:
    CommandReporter(std::string command, CommandLauncher *launcher)
            : cmd(command), l(launcher) {
    }

public:
    bool Report(std::string received, std::string approved) const override {
        FileUtils::ensureFileExists(approved);
        std::vector<std::string> fullCommand;
        fullCommand.push_back(cmd);
        fullCommand.push_back(received);
        fullCommand.push_back(approved);
        return l->Launch(fullCommand);
    }
};

class GenericDiffReporter : public CommandReporter {
private:
    SystemLauncher launcher;

public:
    GenericDiffReporter(const std::string& program) : CommandReporter(program, &launcher) {};
    GenericDiffReporter(const DiffInfo& info) : CommandReporter(info.getProgramForOs().c_str(), &launcher) {};
};

class TestReporter : public CommandReporter {
public:
    DoNothingLauncher launcher;

    TestReporter(bool working = true) : CommandReporter("fake", &launcher) {
        launcher.working = working;
    };
};


#endif

 // ******************** From: FileApprover.h
#ifndef FILEAPPROVER_H
#define FILEAPPROVER_H


class FileApprover {
public:
    FileApprover() {};

    ~FileApprover() {};

    static ApprovalException *verify(std::string receivedPath,
                                     std::string approvedPath) {
        int asize = FileUtils::fileSize(approvedPath);

        if (-1 == asize) {
            return new ApprovalMissingException(receivedPath, approvedPath);
        }

        int rsize = FileUtils::fileSize(receivedPath);

        if (-1 == rsize) {
            return new ApprovalMissingException(approvedPath, receivedPath);
        }

        if (asize != rsize) {
            return new ApprovalMismatchException(receivedPath, approvedPath);
        }

        std::ifstream astream(approvedPath.c_str(),
                              std::ios::binary | std::ifstream::in);
        std::ifstream rstream(receivedPath.c_str(),
                              std::ios::binary | std::ifstream::in);

        while (astream.good() && rstream.good()) {
            int a = astream.get();
            int r = rstream.get();

            if (a != r) {
                return new ApprovalMismatchException(receivedPath, approvedPath);
            }
        }

        remove(receivedPath.c_str());
        return NULL;
    }


    static void verify(ApprovalNamer n, StringWriter s, const Reporter& r) {
        std::string approvedPath = n.getApprovedFile(s.GetFileExtension());
        std::string receivedPath = n.getReceivedFile(s.GetFileExtension());
        s.Write(receivedPath);
        ApprovalException *ae = verify(receivedPath, approvedPath);

        if (ae != NULL) {
            r.Report(receivedPath, approvedPath);
            ApprovalException e(*ae);
            delete ae;
            throw e;
        } else {
            remove(receivedPath.c_str());
        }
    }


};

#endif

 // ******************** From: Approvals.h
#ifndef APPROVALS_H
#define APPROVALS_H


class Approvals
{
private:
    Approvals() {}
    ~Approvals() {}
public:
    static void verify( std::string contents,const Reporter& reporter = DiffReporter() )
    {
        StringWriter writer( contents );
        ApprovalNamer namer;
        FileApprover::verify(namer, writer, reporter);
    }
};

#endif

 // ******************** From: FirstWorkingReporter.h
#ifndef CATCHPLAYGROUND_FIRSTWORKINGREPORTER_H
#define CATCHPLAYGROUND_FIRSTWORKINGREPORTER_H


class FirstWorkingReporter : public Reporter
{
private:
    std::vector< std::unique_ptr<Reporter> > reporters;
public:
    
    FirstWorkingReporter(std::vector<Reporter*> theReporters)
    {
        for(auto r : theReporters)
        {
            reporters.push_back(std::unique_ptr<Reporter>(r));
        }
    }

    bool Report(std::string received, std::string approved) const override
    {
        for(auto& r : reporters)
        {
            if (r->Report(received, approved))
            {
                return true;
            }
        }
        return false;
    }
};

#endif 

 // ******************** From: WindowsReporters.h
#ifndef APPROVALTESTS_CPP_WINDOWSREPORTERS_H
#define APPROVALTESTS_CPP_WINDOWSREPORTERS_H


namespace Windows {
    class BeyondCompare3Reporter : public GenericDiffReporter {
    public:
        BeyondCompare3Reporter() : GenericDiffReporter(DiffPrograms::Windows::BEYOND_COMPARE_3()) {};
    };

    class BeyondCompare4Reporter : public GenericDiffReporter {
    public:
        BeyondCompare4Reporter() : GenericDiffReporter(DiffPrograms::Windows::BEYOND_COMPARE_4()) {};
    };

    class BeyondCompareReporter : public FirstWorkingReporter {
    public:
        BeyondCompareReporter() : FirstWorkingReporter({new BeyondCompare4Reporter(), new BeyondCompare3Reporter()}) {
        }
    };

    class TortoiseImageDiffReporter : public GenericDiffReporter {
    public:
        TortoiseImageDiffReporter() : GenericDiffReporter(DiffPrograms::Windows::TORTOISE_IMAGE_DIFF()) {};
    };

    class TortoiseTextDiffReporter : public GenericDiffReporter {
    public:
        TortoiseTextDiffReporter() : GenericDiffReporter(DiffPrograms::Windows::TORTOISE_TEXT_DIFF()) {};
    };

    class TortoiseDiffReporter : public FirstWorkingReporter {
    public:
        TortoiseDiffReporter() : FirstWorkingReporter(
                {new TortoiseImageDiffReporter(), new TortoiseTextDiffReporter()}) {
        }
    };

    class WinMergeReporter : public GenericDiffReporter {
    public:
        WinMergeReporter() : GenericDiffReporter(DiffPrograms::Windows::WIN_MERGE_REPORTER()) {};
    };

    class AraxisMergeReporter : public GenericDiffReporter {
    public:
        AraxisMergeReporter() : GenericDiffReporter(DiffPrograms::Windows::ARAXIS_MERGE()) {};
    };

    class CodeCompareReporter : public GenericDiffReporter {
    public:
        CodeCompareReporter() : GenericDiffReporter(DiffPrograms::Windows::CODE_COMPARE()) {};
    };

    class KDiff3Reporter : public GenericDiffReporter {
    public:
        KDiff3Reporter() : GenericDiffReporter(DiffPrograms::Windows::KDIFF3()) {};
    };

    class WindowsDiffReporter : public FirstWorkingReporter {
    public:
        WindowsDiffReporter() : FirstWorkingReporter(
                {
                        new TortoiseDiffReporter(),
                        new BeyondCompareReporter(),
                        new WinMergeReporter(),
                        new AraxisMergeReporter(),
                        new CodeCompareReporter(),
                        new KDiff3Reporter()
                }
        ) {
        }
    };
}

#endif 

 // ******************** From: LinuxReporters.h
#ifndef APPROVALTESTS_CPP_LINUXREPORTERS_H
#define APPROVALTESTS_CPP_LINUXREPORTERS_H


namespace Linux
{
    class KDiff3Reporter : public GenericDiffReporter {
    public:
        KDiff3Reporter() : GenericDiffReporter(DiffPrograms::Linux::KDIFF3()) {};
    };

    class MeldReporter : public GenericDiffReporter {
    public:
        MeldReporter() : GenericDiffReporter(DiffPrograms::Linux::MELD()) {};
    };

    class LinuxDiffReporter : public FirstWorkingReporter
    {
    public:
        LinuxDiffReporter() : FirstWorkingReporter(
                {
                        new MeldReporter(),
                        new KDiff3Reporter()
                }
        )
        {
        }
    };

}

#endif 

 // ******************** From: MacReporters.h
#ifndef APPROVALTESTS_CPP_MACREPORTERS_H
#define APPROVALTESTS_CPP_MACREPORTERS_H



namespace Mac {
    class DiffMergeReporter : public GenericDiffReporter {
    public:
        DiffMergeReporter() : GenericDiffReporter(DiffPrograms::Mac::DIFF_MERGE()) {}
    };

    class BeyondCompareReporter : public GenericDiffReporter {
    public:
        BeyondCompareReporter() : GenericDiffReporter(DiffPrograms::Mac::BEYOND_COMPARE()) {}
    };

    class KaleidoscopeReporter : public GenericDiffReporter {
    public:
        KaleidoscopeReporter() : GenericDiffReporter(DiffPrograms::Mac::KALEIDOSCOPE()) {}
    };

    class MacKDiff3Reporter : public GenericDiffReporter {
    public:
        MacKDiff3Reporter() : GenericDiffReporter(DiffPrograms::Mac::KDIFF3()) {}
    };

    class P4MergeReporter : public GenericDiffReporter {
    public:
        P4MergeReporter() : GenericDiffReporter(DiffPrograms::Mac::P4MERGE()) {}
    };

    class TkDiffReporter : public GenericDiffReporter {
    public:
        TkDiffReporter() : GenericDiffReporter(DiffPrograms::Mac::TK_DIFF()) {}
    };

    class MacDiffReporter : public FirstWorkingReporter {
    public:
        MacDiffReporter() : FirstWorkingReporter(
                {
                        new BeyondCompareReporter(),
                        new DiffMergeReporter(),
                        new KaleidoscopeReporter(),
                        new P4MergeReporter(),
                        new MacKDiff3Reporter(),
                        new TkDiffReporter()
                }
        ) {
        }
    };
}

#endif 

 // ******************** From: ApprovalTests.h




#ifndef CATCHPLAYGROUND_APPROVALTESTS_H_H
#define CATCHPLAYGROUND_APPROVALTESTS_H_H


#endif 

 // ******************** From: Catch2Approvals.h

#ifndef CATCHPLAYGROUND_CATCH2APPROVALS_H_H
#define CATCHPLAYGROUND_CATCH2APPROVALS_H_H

// <SingleHpp unalterable>
#ifdef APPROVALS_CATCH
#define CATCH_CONFIG_MAIN
#endif

#include <Catch.hpp>
// </SingleHpp>

using std::string;

#ifdef APPROVALS_CATCH

struct Catch2ApprovalListener : Catch::TestEventListenerBase {
    using TestEventListenerBase::TestEventListenerBase; 
    TestName currentTest;

    virtual void testCaseStarting(Catch::TestCaseInfo const &testInfo) override {
        
        currentTest.fileName = testInfo.lineInfo.file;
        currentTest.testCase = testInfo.name;
        ApprovalNamer::currentTest(&currentTest);
    }

    virtual void testCaseEnded(Catch::TestCaseStats const &testCaseStats) override {
        while (!currentTest.sections.empty()) {
            currentTest.sections.pop_back();
        }
    }

    virtual void sectionStarting(Catch::SectionInfo const &sectionInfo) override {
        currentTest.sections.push_back(sectionInfo.name);
    }

    virtual void sectionEnded(Catch::SectionStats const &sectionStats) override {
        currentTest.sections.pop_back();
    }
};

CATCH_REGISTER_LISTENER(Catch2ApprovalListener)

#endif
#endif 

