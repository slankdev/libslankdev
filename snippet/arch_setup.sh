

### 手順の確認
#インストールは以下の手順である。
# - ベース構築
#	 - パーティショニング
#	 - パーティションをフォーマット
#	 - パーティションをマウント
# - Arch環境の構築
#	 - fstab
#	 - chroot
#	 - ロケール設定
#	 - タイムゾーン設定
#	 - ホストネーム設定
#	 - rootパスワード設定
#	 - pacman設定
#	 - 各ドライバをインストール
# - ブート関係の環境構築
#	 - grubのインストール
#	 - 設定
# - 再起動

check() {
	if [ $# -ne 1 ]; then
		echo "check: Invalid argument"
		exit -1
	fi

	echo -n "$1 [Y/n] "
	read yn
	if [ ${yn} != "Y" ]; then
		echo "Stop"
		exit -1
	fi
}


do_some() {
	if [ $# -ne 3 ]; then
		echo "check: Invalid argument"
		exit -1
	fi
	
	echo -n $2
	$1
	echo $3
}


set_mirrorlist() {
	MIRRORLIST="mirrorlist"
	mv $MIRRORLIST $MIRRORLIST.orignal
	echo "## SetMirror by Slank" >> $MIRRORLIST
	echo "Server = http://ftp.tsukuba.wide.ad.jp/Linux/archlinux/$repo/os/$arch" >> $MIRRORLIST
	echo "Server = http://ftp.jaist.ac.jp/pub/Linux/ArchLinux/$repo/os/$arch" >> $MIRRORLIST
	cat $MIRRORLIST.orignal >> $MIRRORLIST
}


generate_fstab() {
	FSTAB="fstab"
	genfstab -U -p /mnt >> $FSTAB
}


arch_change_root() {
	# arch-chroot /mnt /bin/bash
	echo -n ""
}

set_locate() {
	LOCALE_GEN="locale.gen"
	LOCALE_CONF="locale.conf"

	echo "en_US.UTF-8 UTF-8"  >> $LOCALE_GEN
	echo "ja_JP.UTF-8 UTF-8"  >> $LOCALE_GEN
	locale-gen
	echo "LANG=en_US.UTF-8" >> $LOCALE_CONF
	export LANG=en_US.UTF-8
}


set_timezone() {
	ln -s /usr/share/zoneinfo/Asia/Tokyo /etc/localtime
	hwclock --systohc --utc
}

set_keyboard() {
	VCONSOLE_CONF="vconsole.conf"

	# loadkeys jp106
	# echo "FONT=Lat2-Terminus16" >> $VCONSOLE_CONF
	# echo KEYMAP=jp106 >> $VCONSOLE_CONF
	echo -n ""
}

set_rootpass() {
	passwd
}

conf_pacman() {
	PACMAN_CONF="pacman.conf"

	echo "[multilib]" >> $PACMAN_CONF
	echo "Include = /etc/pacman.d/mirrorlist" >> $PACMAN_CONF
	pacman -Syu
	pacman -S intel-ucode
}


setup_grub() {
	pacman -S dosfstools efibootmgr
	pacman -S grub
	grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=arch_grub --recheck
	mkdir /boot/EFI/boot
	cp /boot/EFI/arch_grub/grubx64.efi /boot/EFI/boot/bootx64.efi
	grub-mkconfig -o /boot/grub/grub.cfg
}

finally() {
	exit
	umount -R /mnt
}






HOSTNAME="myhost"

# check "Have you done disk partitioning?" &&
# check "Have you format partitions?"      &&
# do_some set_mirrorlist   "Set Default JapaneseMirrorServer... " "done"
# do_some generate_fstab   "Generate fstab... "                   "done"
# do_some arch_change_root "Change root... "                      "done"
# do_some set_locate "Set locale... " "done" 
# do_some set_timezone "Set TimeZone... " "done" 
# echo $HOSTNAME > /etc/hostname
# do_some set_rootpass "" ""
# do_some conf_pacman "Configuration pacman... " "done"
# do_some setup_grub "Setup Grub... " "done"
# do_some finally "Finally... " "done"


echo "All task is finished. please reboot"


