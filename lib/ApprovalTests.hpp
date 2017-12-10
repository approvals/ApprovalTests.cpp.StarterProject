#include <string>
#include <fstream>
#include <cstring>
#include <exception>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <sys/stat.h>
#include <stack>
#include <sstream>

#ifdef APPROVALS_CATCH
#define CATCH_CONFIG_MAIN
#endif

#include <Catch.hpp>
// ******************** From: StringWriter.h
#ifndef STRINGWRITER_H
#define STRINGWRITER_H


class StringWriter {
private:
    std::string s;
    std::string ext;

public:
    StringWriter(std::string contents, std::string fileExtensionWithDot = ".txt")
            : s(contents), ext(fileExtensionWithDot) {}

    std::string GetFileExtension() {
        return ext;
    }

    void Write(std::string path) {
        std::ofstream out(path.c_str(), std::ofstream::out);
        this->Write(out);
        out.close();
    }

    void Write(std::ostream &out) {
        out << s << "\n";
    }


};

#endif

// ******************** From: ApprovalException.h
#ifndef APPROVALEXCEPTION_H
#define APPROVALEXCEPTION_H


class ApprovalException : public std::exception {
private:
    char *message;
protected:
    char *init(const char *msg) {
        size_t message_sz = std::strlen(msg);
        char *t = new char[message_sz + 1];
        std::strncpy(t, msg, message_sz + 1);
        return t;
    };
public:
    ApprovalException(const char *msg) : message(init(msg)) {}

    ApprovalException(const ApprovalException &a)
            : message(init(a.message)) {
    }

    virtual ~ApprovalException() throw() {
        delete[] message;
    }

    virtual const char *what() const throw() {
        return message;
    }
};

class ApprovalMismatchException : public ApprovalException {
private:
    char *format(const std::string &received, const std::string &approved) {
        size_t n = 2048;
        char s[n];
        int size = snprintf(s,
                            n,
                            "Failed Approval: \n"
                                    "Received does not match approved \n"
                                    "Received : \"%s\" \n"
                                    "Approved : \"%s\"",
                            received.c_str(),
                            approved.c_str());
        char *t = new char[size + 1];
        std::strncpy(t, s, size + 1);
        return t;
    }

public:
    ApprovalMismatchException(std::string received, std::string approved)
            : ApprovalException(format(received, approved)) {
    }

    ApprovalMismatchException(const ApprovalMismatchException &a)
            : ApprovalException(a) {}
};

class ApprovalMissingException : public ApprovalException {
private:
    char *format(const std::string &file) {
        size_t n = 1024;
        char s[n];
        int size = snprintf(s,
                            n,
                            "Failed Approval: \n"
                                    "Approval File Not Found \n"
                                    "File: \"%s\"",
                            file.c_str());
        char *t = new char[size + 1];
        std::strncpy(t, s, size + 1);
        return t;
    }

public:
    ApprovalMissingException(std::string received, std::string approved)
            : ApprovalException(format(approved)) {
    }

    ApprovalMissingException(const ApprovalMissingException &a)
            : ApprovalException(a) {
    }
};

#endif

// ******************** From: CommandLauncher.h
#ifndef COMMANDLAUNCHER_H
#define COMMANDLAUNCHER_H


class CommandLauncher {
public:
    virtual ~CommandLauncher() {}

    virtual bool Launch(std::vector<std::string> argv) = 0;
};

class DoNothingLauncher : public CommandLauncher {
private:
    std::string cmd;
public:
    bool working = true;

    bool Launch(std::vector<std::string> argv) {
        for (std::vector<std::string>::iterator it = argv.begin();
             it != argv.end();
             ++it) {
            cmd += *it;
            cmd += " ";
        }
        return working;
    }

    std::string ReceivedCommand() {
        return cmd;
    }
};

class SystemLauncher : public CommandLauncher {
public:
    bool exists(const std::string &command) {
        std::string which = "which " + command + " > /dev/null 2>&1";
        int result = system(which.c_str());
        return (result == 0);
    }

    bool Launch(std::vector<std::string> argv) {
        if (!exists(argv.front())) {
            return false;
        }

        char *ARGV[argv.size() + 1];
        int count = 0;

        for (std::vector<std::string>::iterator it = argv.begin();
             it != argv.end();
             ++it) {
            char *cstr = new char[it->length() + 1];
            std::strcpy(cstr, it->c_str());
            ARGV[count] = cstr;
            count++;
        }

        ARGV[argv.size()] = NULL;

        if (fork() == 0) {
            int i = execvp(ARGV[0], ARGV);

            if (i < 0) {
                std::cout << ARGV[0] << ": command not found" << std::endl;
                exit(1);
            }
        }

        for (unsigned int i = 0; i < argv.size(); ++i) {
            delete ARGV[i];
        }

        return true;
    }
};

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
        auto start = file.rfind("/") + 1;
        auto end = file.rfind(".");
        return file.substr(start, end - start);
    }

    string getDirectory() {
        auto file = currentTest().fileName;
        auto end = file.rfind("/") + 1;
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
    GenericDiffReporter(const std::string &program) : CommandReporter(program, &launcher) {};
};

class MeldReporter : public GenericDiffReporter {
public:
    MeldReporter() : GenericDiffReporter("meld") {};
};

class DiffMergeReporter : public GenericDiffReporter {
public:
    DiffMergeReporter() : GenericDiffReporter("/Applications/DiffMerge.app/Contents/MacOS/DiffMerge") {};
};

class TestReporter : public CommandReporter {
public:
    DoNothingLauncher launcher;

    TestReporter(bool working = true) : CommandReporter("fake", &launcher) {
        launcher.working = working;
    };
};

class ReporterFactory {
private:
    ReporterFactory() {}

    ~ReporterFactory() {}

public:
    STATIC(Reporter, currentReporter, new MeldReporter())

    static Reporter &getCurrentReporter() {
        return currentReporter();
    }


    template<typename ReporterType>
    static ReporterType &UseReporter() {
        delete &getCurrentReporter();
        ReporterType *t = new ReporterType();
        return currentReporter(t);
    }
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


    static void verify(ApprovalNamer n, StringWriter s, const Reporter &r) {
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
            std::cout << "Removing " << receivedPath;
            remove(receivedPath.c_str());
        }
    }


};

#endif

// ******************** From: FirstWorkingReporter.h
#ifndef CATCHPLAYGROUND_FIRSTWORKINGREPORTER_H
#define CATCHPLAYGROUND_FIRSTWORKINGREPORTER_H


class FirstWorkingReporter : public Reporter {
private:
    std::vector<Reporter *> reporters;
public:
    FirstWorkingReporter(std::vector<Reporter *> theReporters) : reporters(theReporters) {
    }

    bool Report(std::string received, std::string approved) const override {
        for (auto r : reporters) {
            if (r->Report(received, approved)) {
                return true;
            }
        }
        return false;
    }
};

#endif

// ******************** From: DiffReporter.h
#ifndef CATCHPLAYGROUND_DIFFREPORTER_H
#define CATCHPLAYGROUND_DIFFREPORTER_H


class MacDiffReporter : public FirstWorkingReporter {
public:
    MacDiffReporter() : FirstWorkingReporter({new DiffMergeReporter()}) {
    }
};

class LinuxDiffReporter : public FirstWorkingReporter {
public:
    LinuxDiffReporter() : FirstWorkingReporter({new MeldReporter()}) {
    }
};

class WindowsDiffReporter : public FirstWorkingReporter {
public:
    WindowsDiffReporter() : FirstWorkingReporter({}) {
    }
};

class DiffReporter : public FirstWorkingReporter {
public:
    DiffReporter() : FirstWorkingReporter({new MacDiffReporter(), new LinuxDiffReporter(), new WindowsDiffReporter()}) {
    }

};

#endif

// ******************** From: Approvals.h
#ifndef APPROVALS_H
#define APPROVALS_H


class Approvals {
private:
    Approvals() {}

    ~Approvals() {}

public:
    static void verify(std::string contents, const Reporter &reporter = DiffReporter()) {
        StringWriter writer(contents);
        ApprovalNamer namer;
        FileApprover::verify(namer, writer, reporter);
    }
};

#endif

// ******************** From: ApprovalTests.h




#ifndef CATCHPLAYGROUND_APPROVALTESTS_H_H
#define CATCHPLAYGROUND_APPROVALTESTS_H_H


#endif

// ******************** From: Catch2Approvals.h
#ifndef CATCHPLAYGROUND_CATCH2APPROVALS_H_H
#define CATCHPLAYGROUND_CATCH2APPROVALS_H_H


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

