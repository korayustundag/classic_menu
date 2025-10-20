# Classic Right-Click Menu for Windows 11
A Windows application that restores the classic Windows 10 style right-click context menu in Windows 11. Available in both command-line and graphical user interface versions.
## Description
This utility allows you to easily switch between the new Windows 11 context menu and the classic Windows 10 style right-click menu by modifying the appropriate registry settings. The application comes in two versions:
- **CLI Version**: Command-line interface for quick scripting and advanced users
- **GUI Version**: User-friendly graphical interface with one-click operation
## Features
### Both Versions
-   **Toggle Between Menus**: Easily enable or disable the classic right-click menu
-   **Automatic Explorer Restart**: Option to restart Windows Explorer to apply changes immediately
- **Safe Registry Modifications**: Uses standard Windows registry keys for menu customization
- **Lightweight**: Small executables with minimal dependencies
### GUI Version Specific
- **User-Friendly Interface**: Simple two-button interface
- **Visual Feedback**: Confirmation dialogs and status messages
- **Optional Explorer Restart**: Choose whether to restart Explorer automatically
- **Modern UI**: Clean Windows 11-style interface
## Usage
### Command-Line Version
### Enable Classic Menu
```
classic_menu.exe on
```
### Enable New Windows 11 Menu
```
classic_menu.exe off
```
### GUI Version
1. Run `classic_menu_gui.exe`
2. Click **"Enable Classic Menu"** to restore the Windows 10 style context menu
3. Click **"Disable Classic Menu"** to return to the Windows 11 default menu
4. Choose whether to restart Explorer automatically when prompted
## Download
You can download pre-built binaries from the [Releases](https://github.com/korayustundag/classic_menu/releases) page.
## How It Works
Both applications modify the following registry key to control the context menu behavior:
```
HKEY_CURRENT_USER\Software\Classes\CLSID\{86ca1aa0-34aa-4e8b-a509-50c905bae2a2}\InprocServer32
```
- **Enable Classic Menu**: Creates the registry key with an empty default value
- **Disable Classic Menu**: Removes the registry key entirely

After modifying the registry, the application automatically terminates and restarts Windows Explorer to apply the changes immediately.
## Security Note
This application makes modifications to the Windows Registry. While these changes are reversible and well-documented, always exercise caution when modifying registry settings.
## Troubleshooting
- If changes don't appear immediately, manually restart Windows Explorer
- Run the application as Administrator if you encounter permission errors
- The application only affects the current user's settings
## Frequently Asked Questions
**Q: Is this safe to use?**  
A: Yes, it uses documented registry modifications that are completely reversible.

**Q: Will this affect system stability?**  
A: No, it only modifies the context menu behavior and doesn't change critical system files.

**Q: Do I need to restart my computer?**  
A: No, simply restarting Windows Explorer is sufficient.

**Q: Can I use both versions?**  
A: Yes, but they perform the same function so there's no need to use both.
## Contributing
Feel free to submit issues and enhancement requests! Both CLI and GUI version improvements are welcome.
## Disclaimer
This utility modifies Windows registry settings. Use at your own risk. The developers is not responsible for any system instability that may occur.
