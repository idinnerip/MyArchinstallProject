# 🧰 SystemRescue Fix Guide
**Essential commands for password reset & boot repair**

---

## 🪟 0. Access Command Line Inside Installed OS (from SystemRescue)

### 🐧 Access Linux Command Line (via chroot)

To enter your installed Linux system from SystemRescue:
```bash
lsblk -f                   # Find Linux root partition (ext4, btrfs, etc.)
mount /dev/sda2 /mnt       # Replace sda2 with your partition
mount --bind /dev /mnt/dev
mount --bind /proc /mnt/proc
mount --bind /sys /mnt/sys
chroot /mnt
```
Now you are *inside your installed Linux OS*. You can run:
```bash
passwd username      # Change user password
update-grub          # Rebuild GRUB config
apt, pacman, dnf...  # Use your distro’s package manager
```
Exit and unmount when done:
```bash
exit
umount -R /mnt
```
✅ You’re back to SystemRescue shell.

---

### 🪟 Access Windows Command Line (via registry tools)

Windows can’t be “entered” like Linux, but you can modify its registry, reset passwords, or replace system files.

Mount your Windows partition:
```bash
lsblk -f
mkdir /mnt/win
mount /dev/sda3 /mnt/win
```

Go to Windows registry folder:
```bash
cd /mnt/win/Windows/System32/config
```

You can then:
- List users:
  ```bash
  chntpw -l SAM
  ```
- Clear or change passwords:
  ```bash
  chntpw -u "UserName" SAM
  ```
- View registry:
  ```bash
  reged -L /mnt/win/Windows/System32/config/SOFTWARE
  ```

> ⚠️ You cannot execute Windows `.exe` programs (like `cmd.exe`) from SystemRescue. To use full Windows command line, boot into a Windows PE or recovery ISO.

| Task | Tool | Works for |
|------|------|------------|
| Full OS shell | `chroot` | 🐧 Linux |
| Edit registry / users | `chntpw`, `reged` | 🪟 Windows |
| Copy or modify files | `mount` | 🐧 + 🪟 both |
| Run actual OS commands | `chroot` / Windows PE | Linux / Windows PE |

---

## 🪟 1. Reset Windows Password

### Step 1: Find the Windows partition
```bash
lsblk -f
```
Look for something like `ntfs` or `Windows`.

### Step 2: Mount it
Example (change `sda3` to your real partition):
```bash
mkdir /mnt/win
mount /dev/sda3 /mnt/win
```

### Step 3: Run chntpw
```bash
cd /mnt/win/Windows/System32/config
chntpw -l SAM          # List users
chntpw -u "UserName" SAM
```
Then in the menu:
- Type `1` to clear password  
- Type `q` to quit  
- Type `y` to save

Finally unmount:
```bash
umount /mnt/win
```

✅ Done — Windows password is now blank.

---

## 🐧 2. Reset Linux Password

### Step 1: Find your root partition
```bash
lsblk -f
```
(e.g. `/dev/sda2` → ext4 or btrfs)

### Step 2: Mount and chroot
```bash
mount /dev/sda2 /mnt
mount --bind /dev /mnt/dev
mount --bind /proc /mnt/proc
mount --bind /sys /mnt/sys
chroot /mnt
```

### Step 3: Change password
```bash
passwd username
```
Enter new password twice, then exit and unmount:
```bash
exit
umount -R /mnt
```

✅ Linux password reset complete.

---

## ⚙️ 3. Repair Windows Boot

### Option 1: Using Boot-Repair-Disk (built-in)
```bash
bootrepair
```
If not found:
```bash
boot-repair
```
Follow the GUI → click “Recommended repair”.

### Option 2: Manual via Windows ISO (optional)
If Boot-Repair fails:
- Boot a Windows USB  
- Choose **Repair your computer → Command Prompt**  
Then:
```cmd
bootrec /fixmbr
bootrec /fixboot
bootrec /scanos
bootrec /rebuildbcd
```

---

## 🐧 4. Repair Linux Boot (GRUB)

### Step 1: Mount system
```bash
mount /dev/sda2 /mnt
mount --bind /dev /mnt/dev
mount --bind /proc /mnt/proc
mount --bind /sys /mnt/sys
chroot /mnt
```

### Step 2: Reinstall GRUB
```bash
grub-install /dev/sda
update-grub
exit
umount -R /mnt
```

✅ Reboot — GRUB menu should return.

---

## 💡 Extra tools
| Tool | Purpose |
|------|----------|
| `gparted` | Partition editor |
| `fsck` | Fix filesystem errors |
| `ntfsfix /dev/sda3` | Repair NTFS partition |
| `smartctl -a /dev/sda` | Check HDD health |
| `memtest` | Memory test |
