import qbs

Project {
    property bool withTests: true
    SubProject {
        filePath: "unittest/unittest.qbs"
        Properties {
            condition: parent.withTests
        }
    }
    references: ["mylib/mylib.qbs"]
}
