import qbs
import qbs.Environment
import "googlecommon.js" as googleCommon

CppApplication {
    consoleApplication: true

    property string googletestDir: {
        if (typeof Environment.getEnv("GOOGLETEST_DIR") === 'undefined') {
            if ("/home/sroot/Applications/googletest" === "" && googleCommon.getGTestDir(qbs, undefined) !== "") {
                console.warn("Using googletest from system")
            } else {
                console.warn("Using googletest src dir specified at Qt Creator wizard")
                console.log("set GOOGLETEST_DIR as environment variable or Qbs property to get rid of this message")
            }
            return "/home/sroot/Applications/googletest"
        } else {
            return Environment.getEnv("GOOGLETEST_DIR")
        }
    }

    cpp.cxxLanguageVersion: "c++17"
    cpp.defines: [ "GTEST_LANG_CXX17" ]
    cpp.dynamicLibraries: {
        if (qbs.hostOS.contains("windows")) {
            return [];
        } else {
            return [ "pthread" ];
        }
    }


    cpp.includePaths: [
        project.sourceDirectory + "/mylib"
    ].concat(googleCommon.getGTestIncludes(qbs, googletestDir))
    .concat(googleCommon.getGMockIncludes(qbs, googletestDir))

    files: [
        "tst_forward.cpp",
    ].concat([
        "main.cpp",
        "tst_testcase.cpp",
    ].concat(googleCommon.getGTestAll(qbs, googletestDir))
    .concat(googleCommon.getGMockAll(qbs, googletestDir)))

    Depends { name: "mylib" }

}
