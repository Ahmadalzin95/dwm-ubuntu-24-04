# Ultimate dwm Setup for Ubuntu 24.04

![Ubuntu](https://img.shields.io/badge/Ubuntu-24.04_LTS-E95420?style=for-the-badge&logo=ubuntu&logoColor=white)
![C](https://img.shields.io/badge/C-Suckless-00599C?style=for-the-badge&logo=c&logoColor=white)
![Bash](https://img.shields.io/badge/Bash-Scripts-4EAA25?style=for-the-badge&logo=gnu-bash&logoColor=white)

This repository contains a fully automated, dynamic desktop environment based on the [dwm](https://dwm.suckless.org/) (dynamic window manager) and [dmenu](https://tools.suckless.org/dmenu/) from Suckless. It is specifically designed and optimized for Ubuntu 24.04 LTS.

The setup features a "Material-Black-Blueberry" aesthetic, powered by [Wallust](https://explosion-mental.codeberg.page/wallust/intro.html) for real-time color generation across the entire system.

> --> This project is provided "as is" and without any warranty. It works perfectly on my three personal laptops, but I cannot guarantee it will work on yours.

## Key Features

* **One-Command Install:** No manual compilation needed.
* **Ubuntu Optimized:** Replaces GNOME bloat effectively.
* **Dynamic Theming:** Wallust integration for instant color switches.
* **Low-Battery Warnings:** Threshold notifications (20/15/10/5%) plus a color-coded battery reading in the bar.
* **Preconfigured Zsh:** Zsh + Oh My Zsh as the default shell, with a prompt that follows the Wallust colors.
* **Scratchpad Terminal:** Toggle a floating dropdown terminal from anywhere with `Alt + o`.
* **Per-Tag Layouts (pertag):** Each tag remembers its own layout, master size, window count and bar state.

## Demo

![dwm - ubuntu 24.04 Demo](assets/demo.gif)

## Performance Benchmark

This setup is optimized for efficiency. Below is a real-world comparison between the standard Ubuntu 24.04 (GNOME) session and this `dwm` suite, tested on the exact same hardware with the same background workload.

Values represent the **Base System Load** (OS + Desktop Environment) without additional user applications opened.

| Metric | GNOME | dwm | Efficiency Gain |
| :--- | :-----: | :-----: | :-----: |
| **Idle RAM Usage** | ~1.3 GB | **~650 MB** | **-50% Memory Usage** |
| **Process Count** | ~280 | **~190** | **~90 Fewer Tasks** |
| **Boot to Desktop** | 50.5s | **48.8s** | **Similar** |

### RAM Usage Analysis

The `~650 MB` idle usage is higher than minimal distributions (like Arch or Void) due to the underlying Ubuntu base services, not the window manager itself.

* **Ubuntu Base:** The remaining memory is reserved by mandatory background services that ensure system stability and hardware compatibility:
    * **Snap Daemon:** `~150 MB` -> Background service for Snap packages.
    * **Systemd Suite:** `~150 MB` -> Journaling, Logind, Resolved.
    * **Network & Audio:** `~100 MB` -> NetworkManager, Pipewire.
    * **Security & Policy:** `~100 MB` -> Polkit, UPower, AccountsService.

Replacing `GNOME` with `dwm` removes the desktop layer overhead, but this base system load remains to maintain the "out-of-the-box" Ubuntu functionality.

## Installation

The setup is designed to be completed with a single command.

1. Clone the repository:

```bash
git clone https://github.com/Ahmadalzin95/dwm-ubuntu-24-04.git ~/suckless
cd ~/suckless
```

2. Run the setup script:
```bash
chmod +x setup.sh
./setup.sh
```
3. Switch Session: Once the script finishes, log out of your current session. At the Ubuntu login screen, click the small gear icon in the bottom right corner, select dwm, and log back in.

## System Scripts

All logic is contained within the [scripts/](https://github.com/Ahmadalzin95/dwm/tree/main/scripts) directory. Here is a brief overview of the custom tools:


| Script | Function |
| ------ | -------- |
| `apply-theme`| Changes wallpaper and forces Wallust to regenerate system colors instantly.|
| `app_manager` | Lists running X11 windows in dmenu and allows you to terminate them. |
| `autostart` | Runs background services and initializes monitor layouts. |
| `bt_status` | Provides Bluetooth connectivity status for the slstatus bar. |
| `dwm-menu` | System hub for WiFi, Bluetooth, VPN, and Power management. |
| `hw_status` | Detects hardware interfaces (`WiFi`, `Battery`, `Temp`) for bar monitoring. |
| `layout_toggle` | Cycles keyboard layouts (`US`, `DE`, `AR`) with system notifications. |
| `mycal` | Interactive calendar with Vim-like navigation (`j`/`k`) inside a terminal popup. |
| `lock` | Activates a blurred lockscreen via [Betterlockscreen](https://github.com/betterlockscreen/betterlockscreen). |
| `display-setup` | Interactive GUI (arandr) to configure and automatically save dynamic monitor profiles. |
| `screenshot` | Advanced screen capture tool for `areas`, `windows`, or `full monitors`. |
| `battery_monitor` | Background daemon that sends low-battery notifications at 20/15/10/5% (critical below 10%). |
| `chromium-tiled-launch` | Launch wrapper that fixes content-size rendering of Chromium-based browsers (Brave, Chrome, …) under the tiling WM. |

## Shortcuts & Control

This configuration uses Alt (Mod1) as the main key and Super (Windows Key) for system functions.

### Navigation & Apps

* `Alt + p`: Open App Launcher (Desktop applications only)
* `Alt + Shift + p`: Open Command Launcher (dmenu_run)
* `Alt + Shift + Enter`: Open Gnome-Terminal
* `Alt + s`: Open System Menu (Power/WiFi/VPN)
* `Alt + x`: Open App Manager (Kill running apps)
* `Alt + F1`: Open Calendar
* `Super + Shift + p`: Open nsxiv Image Gallery
* `Super + m`: Open Interactive Monitor Setup (display-setup)
* `Super + e`: Open File Manager (Nautilus)
* `Super + Shift + e`: Open superfile (`spf`, terminal file manager)
* `Alt + o`: Toggle scratchpad terminal (floating dropdown)

### Window Management

* `Alt + j / k`: Focus next/previous window

* `Alt + Shift + c`: Close focused window

* `Alt + b`: Toggle Status Bar visibility

* `Alt + Enter`: Swap focused window to Master

* `Alt + Ctrl + Shift + q`: Restart dwm

### Hardware & Media

* `Super + Space`: Toggle Keyboard Layout (US -> DE -> AR)

* `Super + l`: Lock Screen

* `PrintScreen`: Capture Full Monitor

* `Super + s`: Capture Selected Area

* `Audio Keys`: Volume Up/Down/Mute (with notifications)

* `Brightness Keys`: Display Brightness Up/Down (with notifications)

## Custom Theming
The system uses a dynamic theming engine. You can change your entire system look by running:
```bash
apply-theme </path/to/your/wallpaper.jpg>
```

## Shell (Zsh)
The setup installs [Zsh](https://www.zsh.org/) with [Oh My Zsh](https://ohmyz.sh/) and sets it as the default login shell. It ships a custom two-line prompt (current directory and Git branch on the left, full path on the right, with a command timer) whose colors are wired to the Wallust palette, so the prompt re-themes together with the rest of the system. The shell configuration lives in `assets/zsh/`.

## Legal & Technical Note
This build is based on source code from [suckless.org](https://suckless.org/). It includes several patches for dwm and dmenu to enable gaps, centering, colored status text (status2d), and desktop file support.
