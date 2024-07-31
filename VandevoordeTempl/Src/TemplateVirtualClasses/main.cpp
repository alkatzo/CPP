#include <iostream>
#include "database.h"

using namespace std;

int main()
{
    Database::make_db();
    Database::the->greet();
    return 0;
}
