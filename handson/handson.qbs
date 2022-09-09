
Project {
    name: "handsOnC++"
    SubProject {
        filePath: "src/unittest/unittest.qbs"
        Properties {
            name: "unittests"
        }
    }
    SubProject {
        filePath: "src/main/main.qbs"
        Properties {
            name: "main"
        }
    }
}
