#include "simulator_libraries/FEHLCD.h"
#include "simulator_libraries/FEHUtility.h"
#include "SeparateFiles/menu.h"
#include <time.h>
#include <stdlib.h>

//Screen boundaries x: 0-319
//Screen Boundaries y: 0-239
int main()
{
    srand(time(0));
    CreateMenu();

    while (1)
    {
        GameFunction();
        LCD.Update();
        
    }
    
    return 0;
}