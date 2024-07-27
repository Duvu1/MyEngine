// MyEngine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "MyEngine.h"
#include "ExampleApp.h"


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    ExampleApp example;

    if (!example.Initialize())
    {
        std::cout << "Failed: Initialize()" << std::endl;
        return -1;
    }

    return example.Run();
}
