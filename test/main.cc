
#include <iostream>

#include "test.h"

int main(int argc, char *argv[])
{
    printf("\nProtocol Test Begin\n");

    Test::test<MessageHeader>();
    Test::test<Veh2CloudInh>();
    Test::test<Cloud2VehInhRes>();
    Test::test<Veh2CloudState>();

    printf("\nProtocol Test End\n");
    
    return 0;
}
