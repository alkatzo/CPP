import qbs.FileInfo

StaticLibrary {
    Depends { name: "cpp" }

    cpp.cxxLanguageVersion: "c++20"
    cpp.defines: [

        // You can make your code fail to compile if it uses deprecated APIs.
        // In order to do so, uncomment the following line.
        //"QT_DISABLE_DEPRECATED_BEFORE=0x060000" // disables all the APIs deprecated before Qt 6.0.0
    ]

    files: [
        "caller.cpp",
        "caller.h",
        "variadic_call.h",
    ]

    // Default rules for deployment.
    qbs.installPrefix: ""
    Properties {
        condition: qbs.targetOS.contains("unix")
        install: true
        installDir: "$$[QT_INSTALL_PLUGINS]/generic"
    }
}
