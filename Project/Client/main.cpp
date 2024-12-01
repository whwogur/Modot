#include "pch.h"
#include "framework.h"
#include "Client.h"

#include "CEditorMgr.h"
#include <Engine/CDevice.h>
#include "ImGui/imgui_impl_win32.h"
#include "CLevelSaveLoad.h"
#include "CTestLevel.h"
// 전역 변수:
HINSTANCE g_hInst = nullptr;


ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    MD_PROFILE_BEGIN_SESSION("Init", "Profile-Initialization.json");
    int screenX = GetSystemMetrics(SM_CXSCREEN);
    int screenY = GetSystemMetrics(SM_CYSCREEN);
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetBreakAlloc(1599134);

    AllocConsole();
    FILE* pConsole;
    freopen_s(&pConsole, "CONOUT$", "w", stdout);

    SetConsoleOutputCP(CP_UTF8);
    
    HWND consoleWindow = GetConsoleWindow();
    SetWindowPos(consoleWindow, nullptr, 0, 768, 600, 300, SWP_NOZORDER);
#endif
    g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    MyRegisterClass(hInstance);

    HWND hWnd = CreateWindowW(L"ModotEngine", L"Modot", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }
    ShowWindow(hWnd, true);
    UpdateWindow(hWnd);

    // CEngine 객체 초기화
    if (FAILED(CEngine::GetInst()->Init(hWnd
        , POINT{ screenX, screenY }
        , (OBJECT_SAVE)&CLevelSaveLoad::SaveGameObject
        , (OBJECT_LOAD)&CLevelSaveLoad::LoadGameObject)))
    {
        MessageBox(nullptr, L"Engine 초기화 실패", L"엔진 초기화 실패", MB_OK);
        return 0;
    }

    CTestLevel::CreateTestLevel();

#ifdef _DEBUG
    CEditorMgr::GetInst()->Init();
#endif
    MD_PROFILE_END_SESSION();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));
    MSG msg = {};
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            MD_PROFILE_BEGIN_SESSION("Runtime", "Profile-Runtime.json");
            CEngine::GetInst()->Tick();
#ifdef _DEBUG
            CEditorMgr::GetInst()->Tick();
#endif
            CDevice::GetInst()->Present();

            MD_PROFILE_END_SESSION();
        }
    }

#ifdef _DEBUG
    FreeConsole();
#endif
    return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"ModotEngine";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}