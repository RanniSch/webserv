#include	"TestClient.cpp"

int main(void)
{

    // Create a client instance and connect to the server
    std::cout << GREEN "Testing custom client!  ===============" BLANK << std::endl;
    TestClient client("127.0.0.1", 8000);
    client.connectToServer();

    // Send an HTTP POST request
    std::string postData = "*** Hello Server from Client! ***";
    client.sendHttpPostRequest("/api/endpoint", postData);
    std::cout << GREEN "Custom Client is done!  ===============" BLANK << std::endl;
}