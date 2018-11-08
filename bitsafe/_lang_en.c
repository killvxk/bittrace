#include "common_func.h"
#include "lang_en.h"

pcchar_t all_strings_en[] = {
	//Main button and title
	__t( "SUMMARY" ), 
		__t( "NETWORK" ), 
		__t( "SYSTEM" ), 
		__t( "HELP" ), 
		__t( "BITSAFE" ), 
		__t( "BITSAFE" ), 
		__t( "Hide(&H)"), 
		__t( "Bitsafe has started" ), 

		//Security logo description
		__t( "multi-function shield is opened.system is protected." ), 
		__t( "multi-function shield is closed.system is weak." ), 
		__t( "Open shield" ), 

		//Help panel
		__t( "Help" ), 
		__t( "Forum" ), 
		__t( "About" ), 
		__t( "Manual update" ), 
		__t( "Give me suggestion" ), 
		__t( "Email remote control" ), 
		__t( "Relearn rule" ), 
		__t( "Option" ), 
		//__t( "Language" ), 
		__t( "Suggestion" ), 
		__t( "Help me by one click" ), 
		__t( "Uninstall" ), 

		//Defense panel
		__t( "On/Off the network control panel" ), 
		__t( "All running applications" ), 
		__t( "Receive system manage power" ), 
		__t( "Defense rules" ), 
		__t( "Trusted files" ), 
		__t( "Distrusted files" ), 
		__t( "System action log" ), 

		//Firewall panel
		__t( "Network action log" ), 
		__t( "Trusted files" ), 
		__t( "Distrusted files" ), 
		__t( "Network rules" ), 
		__t( "Allow/Block ping" ), 
		__t( "All network connections" ), 
		__t( "Anti-Arp attack" ), 
		__t( "User Account" ), 
		__t( "Work mode" ), 

		//Summary panel
		__t( "Firewall" ), 
		__t( "Firewall block " ), 
		__t( " network actions" ), 
		__t( "Uploaded " ), 
		__t( "bytes" ), 
		__t( "Downloaded " ), 
		__t( "bytes" ), 

		__t( "Defense" ), 
		__t( "Defense block " ), 
		__t( " system actions" ), 
		__t( "Monitor the current behavior of this system" ), 

		//slogan
		__t( "Don't tip after bitsafe restart" ), 
		
		__t( "If you use windows xp or previous, when bitsafe work with other security software togather, because some reason,system probably will running slowly, or happen other error.consider about competition between the softwares or some other reason etc,bitsafe will give some security functions to other software." ), 
		__t( "Have 1 unknown bit generated by bitsafe? Just tell me.Have 1 unknown bit generated by other program? let me see.Send your message to: email:codereba@gmail.com or forum." ), 
		__t( "If bitsafe updated to the version which is lower than 2.0.0.288, please first uninstall bitsafe and restart windows system after updated,( don't select yes when show message box for installation ), otherwise will have some error." ), 

		//come error tip
		__t( "BitSafe is supporting 32 bit windows,don't support 64 bit windows." ), 
		__t( "BitSafe must is running normally after windows reboot, else some function can't be loaded." ), 
		__t( "Current user don't have the privilege to do this action, please login first." ), 

		__t( "Skin customize" ), 

		//Common button text
		__t( "OK" ), 
		__t( "Cancel" ), 

		__t( "Yes" ), 
		__t( "No" ), 

		__t( "Confirm" ), 
		__t( "Cancel" ), 

		__t( "Open" ), 
		__t( "Close"), 

		//User login text
		__t( "Login" ), 
		__t( "Login" ), 
		__t( "Password" ), 
		__t( "Click me to login" ), 
		__t( "Change password" ), 
		__t( "Click me to cancel" ), 
		__t( "Password is incorrect"), 
		__t( "Login successfully, you are the administrator now" ), 

		//Change password dialog
		__t( "Two new password input is not same."), 
		__t( "Old password is incorrect."), 
		__t( "Password will be cleared, sure?" ), //__t( "Password length need greater than 8." ), 
		__t( "Change password successfully"), 
		__t( "Password management"), 
		__t( "Old password:"), 
		__t( "Input old password here" ), 
		__t( "New password:"), 
		__t( "Input new password here" ), 
		__t( "Again:" ), 
		__t( "Input new password again" ), 
		__t( "Note: Original password is null,just don't input anything into old password." ), 
		//Change password
		__t( "The configuration data is corrupted,need set user information now." ), 
		__t( "Have not setting the password now, if need to manage student web surf, set it, that will ensures that only you can change setting, else will not use password."), 
		__t( "Ensures that only you can change setting,Everytime,you can enter this UI by click the \"User Account\" button." ), 
		__t( "Set password" ), 

		//Account management dialog
		__t( "Account management" ), 
		__t( "Old password" ), 
		__t( "New password" ), 
		__t( "Again" ), 

		__t( "Notice:Original password is null,just don't input anything into old password" ), 

		//Anti arp dialog
		__t( "Open anti-arp" ), 
		__t( "Close anti-arp" ), 

		__t( "Notice:Arti-arp need system performance, don't use it, if don't need." ), 
		__t( "Answered mac addresses by arp for host %d.%d.%d.%d: \n" ), 
		__t( "Because the context with system or other software, anti-arp function can't load correctly,need to restart system,restart now?" ), 

		//Background work
		__t( "BitSafe has not installed, install it now?" ), 

		//Work mode dialog
		__t( "Work mode setting" ), 
		__t( "Click me select the mode you want" ), 
		__t( "Work mode" ), 
		__t( "Free mode" ), 
		__t( "Control mode" ), 
		__t( "Block all mode" ), 
		//Change work mode
		__t( "Changing work mode error, already uninstalled bitsafe?" ), 
		__t( "Work mode has changed." ), 

		//Response action type
		__t( "Block" ), 
		__t( "Allow" ), 
		__t( "Learn" ), 

		//System action response
		__t( "Trust/Block this application's all actions" ), 
		__t( "Record action type( network, file, registry)" ), 
		__t( "Record this action response" ), 

		//System action type
		__t( "Install driver" ), 
		__t( "Install COM" ), 
		__t( "Modify registry" ), 
		__t( "Modify file" ), 
		__t( "Delete file" ), 
		__t( "Locate to URL" ), 
		__t( "Install hook" ), 
		__t( "Create process" ), 
		__t( "Access other process" ), 
		__t( "Do protected action" ), 
		__t( "Connect to other host" ), 
		__t( "Send data to peer" ), 
		__t( "Receive data from peer" ), 

		//System action dialog
		__t( " allow it?" ), 
		__t( "Application %s load driver %s, " ), 
		__t( "Application %s install system hook, " ), 
		__t( "Application %s create process %s," ), 
		__t( "Application %s access the memory of application %s directly," ), 
		__t( "Application %s install COM %s," ), 
		__t( "Application %s modify the registry in path: %s," ), 
		__t( "Application %s modify the file in path %s," ), 
		__t( "Application %s delete the file in path %s," ), 
		__t( "Application %s communicates with %s," ), 
		__t( "Application %s create the %s socket( %s:%d to %s:%d%s )" ), 
		__t( "Application %s create the %s socket( %s:%d )" ), 
		__t( "Application %s send the packet( %s:%d to %s:%d )" ), 
		__t( "Application %s receives the packet( %s:%d to %s:%d )" ), 
		__t( "Application %s do the protected actions, allow it?" ), 

		//Network action log dialog
		__t( "Process id" ), 
		__t( "Process name" ), 
		__t( "Action" ), 
		__t( "Protocol" ), 
		__t( "Dest (Ip or Url)" ), 
		__t( "Dest port" ), 
		__t( "Source ip" ), 
		__t( "Source port" ), 
		__t( "Response" ), 
		__t( "Description" ), 
		__t( "Time" ), 

		//add trust application
		__t( "Add trust application" ), 
		__t( "Application executable file name:" ), 
		__t( "Browser" ), 
		__t( "Add application rule error, already have uninstalled?" ), 
		__t( "Add application rule successfully" ), 

		//add block application
		__t( "Add block application" ), 

		//url rule tab
		__t( "Application name" ), 
		__t( "Url" ), 
		__t( "Action" ), 
		__t( "Description" ), 
		__t( "Add" ), 
		__t( "Edit" ), 
		__t( "Delete" ), 

		//ip rule tab
		__t( "Source IP begin" ), 
		__t( "Source IP end" ), 
		__t( "Source port begin" ), 
		__t( "Source port end" ), 

		__t( "Dest IP begin" ), 
		__t( "Dest IP end" ), 
		__t( "Dest port begin" ), 
		__t( "Dest port end" ), 

		__t( "Protocol" ), 

		//name edit dialog
		__t( "Operation:" ), 
		__t( "Description:" ), 
		__t( "Application: " ), 

		__t( "Url rule definition" ), 
		__t( "Url or domain name" ), 
		__t( "File rule definition" ), 
		__t( "File path"), 
		__t( "Registry rule definition" ), 
		__t( "Registry path" ), 

		//IP rule edit dialog
		__t( "Operation:" ), 
		__t( "Application:" ), 
		__t( "Description:" ), 
		__t( "Protocol:" ), 
		__t( "Dest port:" ), 
		__t( "Source port:" ), 
		__t( "Dest IP:" ), 
		__t( "Source IP:" ), 
		__t( "TCP" ), 
		__t( "UDP" ), 
		__t( "All" ), 
		__t( "Select protocol" ), 

		//acl edit dialog
		__t( "Description length can't greater than 512." ), 
		__t( "The length of target application path can't greater than 260." ), 
		__t( "The format of begin source ip address isn't correct." ), 
		__t( "The format of end source ip address isn't correct." ), 
		__t( "The format of begin destination ip address isn't correct." ), 
		__t( "The format of end destination ip address isn't correct." ), 
		__t( "The format of begin source port isn't correct." ), 
		__t( "The format of end source port isn't correct." ), 
		__t( "The format of begin destination port isn't correct." ), 
		__t( "The format of end destination port isn't correct." ), 
		__t( "Input error." ), 
		__t( "The source ip and the destination ip can't be null both." ), 
		__t( "The source port and the destination port can't be null both." ), 
		__t( "Add socket rule successfully" ), 

		//acl management
		__t( "Add rule successfully" ), 
		__t( "Edit rule successfully" ), 

		//icon menu 
		__t( "Open" ), 
		__t( "Login" ), 
		__t( "Work mode" ), 
		__t( "Exit" ), 

		//icmp ping dialog
		__t( "ICMP setting" ), 
		__t( "Notice: Other host can't ping this host when you turn on the switch,and vice verse" ), 
		__t( "Block Ping" ), 
		__t( "Allow Ping" ), 
		__t( "Allow ping error, maybe driver isn't loaded, please run application again." ), 
		__t( "Allow ping successfully" ), 
		__t( "Block ping error, maybe driver isn't loaded, please run application again." ), 
		__t( "Block ping successfully" ), 

		//All network connection dialog
		__t( "Process ID" ), 
		__t( "Executable file path" ), 
		__t( "All uploaded" ), 
		__t( "All downloaded" ), 
		__t( "Block status" ), 
		__t( "Upload speed" ), 
		__t( "Upload speed limit" ), 
		__t( "download speed" ), 
		__t( "BlkUp/" ), 
		__t( "BlkDown" ), 
		__t( "AllowUp/" ), 
		__t( "AllowDown" ), 
		__t( "Up speed" ), 
		__t( "Please select the target by click" ), 
		//__t( "TCP" ), 
		//__t( "UDP" ), 
		//__t( "*" ), 
		
		//All strings in defense panel
		//Defense log dialog
		__t( "Application" ), 
		__t( "Action" ), 
		__t( "Target" ), 
		__t( "Response" ), 
		__t( "Time" ), 

		//All running process panel
		__t( "Process id" ), 
		__t( "Process name" ), 
		__t( "Description" ), 

		__t( "Add as trusted" ), 
		__t( "Add as distrusted" ), 

		//File system rule panel
		__t( "Application name" ), 
		__t( "File/Directory path" ), 
		__t( "Action" ), 
		__t( "Description" ), 

		//Registry rule panel
		//__t( "Application name" ), 
		__t( "Registry key path" ), 
		//__t( "Action" ), 
		//__t( "Description" ), 

		//rule configuration dialog 
		__t( "Select one rule first" ), 
		__t( "Default rule is important for security of the system, deleting it will introduce security vulnerability." ), 
		__t( "Delete this rule successfully" ), 

		//Get defense power
		__t( "Notice: Recommended to use other security application which have more function for defense.like safe360.This function need reboot" ), 
		__t( "Get defense power from other application" ), 
		__t( "Acquire" ), 
		__t( "Release" ), 

		//Network control panel management
		__t( "Switch network control panel" ), 
		__t( "Close panel" ), 
		__t( "Open panel" ), 
		__t( "Will close the network control panel for the security of the network configuration, Notice:Because system environment,sometime open the control panel need system restart." ), 
		__t( "Open network control panel error, maybe is not compatible with the system, please try again." ), 
		__t( "Open network control panel successfully" ), 
		__t( "Close network control panel error, maybe is not compatible with the system, please try again." ), 
		__t( "Close network control panel successfully" ), 

		//Relearn message
		__t( "Relearn acl" ), 
		__t( "Reset acl successfully" ), 

		//Erc dialog
		__t( "Email remote control ( ERC )" ), 
		__t( "Operation" ), 
		__t( "Block all communication" ), 
		__t( "Hold time:" ), 
		__t( "Hour " ), 
		__t( "Minute" ), 
		__t( "Send command" ), 
		__t( "Setting" ), 
		__t( "Open ERC service" ), 
		__t( "Close ERC service" ), 
		__t( "Notice:Please read me first" ), 
		__t( "Click here to select the remote command" ), 
		__T( "ERC command" ), 

		//Email account setting dialog
		__t( "Display name " ), 
		__t( "( Just for display ) " ), 
		__t( "Email account " ), 
		__T( "(e.g.john@sohu.com ) " ), 
		__t( "Password " ), 
		__t( "Smtp server name/ip " ), 
		__t( "(e.g.smtp.sohu.com )" ), 
		__t( "Smtp server port " ), 
		__t( "(Default 25" ), 
		__t( "Pop3 server name/ip (e.g.pop.sohu.com )" ), 
		__t( "Pop3 server port (Default 110" ), 
		__t( "Test" ), 
		__t( "Confirm" ), 
		__t( "sohu.com smtp server and pop3 server have tested,is recommended,and remote control email will delete automatically,notice don't use the \"REMOTE COMMAND\" to be the starting text on the email content" ), 
		__t( "The length of display name can't greater than 260." ), 
		__t( "The length of email account can't greater than 260." ), 
		__t( "The format of email account is not correct." ), 
		__t( "The email account must be set." ), 
		__t( "The length of name of stmp server can't greater than 260." ), 
		__t( "The name of stmp server must be setting." ), 
		__t( "The length of name of pop3 server can;t greater than 260." ), 
		__t( "The name of pop3 server must be setting" ), 
		__t( "The length of password can't greater than 260." ), 
		__t( "No need password?Please confirm." ), 
		__t( "The format of SMTP port isn't correct." ), 
		__t( "The format of POP3 port isn't correct." ), 

		__t( "Test email account successfully." ), 
		__t( "Test email account error" ), 
		__t( "Receive the allow all remote command." ), 
		__t( "Receive the block all remote command." ), 
		__t( "Receive the acl control remote command." ), 
		__t( "%s, hold time is:%I64u seconds\n" ), 
		__t( "Send remote command successfully" ), 
		__t( "Send remote command error" ), 
		__t( "Previous command is not sent, the command must be sending in order." ), 
		__t( "Load the ERC configuration error, maybe is that the configuration file to be corrupted, please setting it now." ), 
		__t( "Email account for erc has set." ), 
		__t( "Email account setting" ), 
		__t( "Time is must be setting."), 
		__t( "Email account of ERC have not setting, set it now?" ), 

		//ERC notice dialog
		__t( "Please notice: \n" ) \
		__t( "1.Remote control implemented by email,client send the command be encapsulated by email, and server receive and execute it.\n" ) \
		__t( "2.Remote control is depend on the smtp and pop3 server, so the delay time can't exactly sure.general serveral minutes\n" ) \
		__t( "3.Sometime smtp server or pope3 server can't receive command, please try again.\n" ) \
		__t( "4.Recover acl mode when command time is end.\n" ) \
		__t( "5.If you send block all command, all communication will closed, include communication for remote control\n" ) \
		__t( "6.Notice: Don't let the stmpt server and the pop3 server conflict with the url rule.\n" ) \
		__t( "7.sohu.com email is tested\n" ) \
		__t( "8.Remote control email will deleting automatic,notice don't use the \"REMOTE COMMAND\" to be the starting text on the email content, recommend independent email account\n" ) \
		__t( "9.One remote control command will be stopped by later received command,so don't send too more time( recommend 2 time ).\n" ) \
		__t( "10.You can check the remote control command email in your account,if it treated wrong,correct it\n" ) \
		__t( "11.We develope free software, please support us, we will develope more enhanced functions\n" ) \
		__t( "\n") \
		__t( "\nContact us by email:" ) BITSAFE_EMAIL __t( " or http://www.codereba.com/bbs\n" ), 

		//About dialog
		__t( "Codereba BitSafe(SevenFw) V2.0" ), 
		__t( "(C) 2009-2012 Codereba" ), 
		__t( "All right reserved." ), 
		__t( "This software is protected by china and international copyright and intellectual property laws, if reverse engine,crack, modify or use it for some commercial purpose without permit by balancesoft, will get the maximum punishment by law." ), 

		//Language dialog
		__t( "Select language" ), 
		__t( "Language:"), 
		__t( "Click here to select the language" ), 
		__t( "Set language error\n" ), 
		__t( "language has set\n" ), 
		__t( "Load language configuration error" ), 
		__t( "Language configuration file is corrupted, need setting again." ), 
		__t( "Have not set language now, please set it now, then ui can display correctly" ), 
		//__t( "Set language" ), 
		//__t( "Set language successfully" ), 
		//__t( "Set language error" ), 

		//update dialog
		__t( "Updating" ), 
		__t( "File downloaded: %d/%d" ), 
		__t( "Updating error" ), 
		__t( "Updating successful" ), 
		__t( "BitSafe configuration file is corrupted, reinstall bitsafe can fix it." ), 
		__t( "Get updating information error, please check network,if that's correct, you download manually from:\"http://www.codereba.com/bitsafev2.zip\" wait server have maintained." ), 
		__t( "Updating server is maintained, can't be updating now." ), 
		__t( "Updating server configuration is error, please report it to administrator ( email:" ) BITSAFE_EMAIL __t( " bbs:codereba.com/bbs ), Thank you." ), 
		__t( "You are using newest version, don't need to update." ), 
		__t( "Old version have not uninstall successfully, uninstall it before you reinstall it and run it ( don't select yes when show message box for installation." ), 
		__t( "Is updating %d file: %s" ), 
		__t( "Already is newest version %s, don't need to update" ), 
		__t( "Have checked newer version %s (%s), update to it?" ), 
		__t( "Updating have started,all files count %d " ), 

		//suggestion dialog
		__t( "Give suggestion, I will do better for you, that's best help." ), 
		__t( "When suggestion is accepted, you will record to the list of support people of balancesoft,and you will get the special version bitsafe.( your photo or name input to ui )." ), 
		__t( "Send" ), 
		__t( "Send suggestion successfully" ), 

		//System action tip dialog
		__t( "Specious system action" ), 

		//Uninstallation
		__t( "Uninstall bitsafe is needing to restart system  restart now?" ), 
		__t( "Uninstall bitsafe error, is already uninstalled?" ), 

		//Installation
		__t( "System or other security software is not compatible with bitsafe, or bitsafe file is corrupt, please run again after restart,still error, then use original packet reinstall bitsafe.if still have error, please send to me( " ) BITSAFE_EMAIL __t( ", http://codereba.com/bbs, or suggestion ),thanks." ), 
		__t( "Installation need system restart, restart now?" ), 
		__t( "Installation error, because the compatiblility with other application or system context, restart system now or later?" ), 

		__t( "Load security configuration error, please try to run again, if still have error, please send to me( " ) BITSAFE_EMAIL __t( ", http://codereba.com/bbs or suggestion ),thanks" ), 
		__t( "Load security configuration error, please reinstall it and try again, if still have error, please send to me( " ) BITSAFE_EMAIL __t( ", http://codereba.com/bbs, or suggestion ),thanks" ), 

		//Help me dialog
		__t( "When you click this button, your IE main page will set to this web page, that will help me." ), 
		__t( "Notice: This is one chinese web site, please choice." ), 

		//Skin customizing dialog
		__t( "Transparent:" ), 
		__t( "change skin" ), 

		//Run-time log dialog
		__t( "Run time log" ), 
		__t( "Common message" ), 
		__t( "File message" ), 
		__t( "Registry message" ), 
		__t( "Network message" ), 

		//bitsafe setting

		__t( "Let bitsafe auto run when windows started?" ), 

		//option dialog
		__t( "Option" ), 
		__t( "UI" ), 
		__t( "Security" ), 

		//security option dialog
		__t( "URL firewall" ), 
		__t( "ARP firewall" ), 
		__t( "Base firewall " ), 
		__t( "Traffic firewall" ), 
		__t( "Defense" ), 
		__t( "File system shield" ), 
		__t( "Registry shield"), 

		__t( "Defend internet surfing from website" ), 
		__t( "Defend network from blocking by arp-attack" ), 
		__t( "Protect the functions of network safe" ), 
		__t( "Protect clearing and trusting of all network commucation" ), 
		__t( "Protect other system action safe" ), 
		__t( "Protect file io safe" ), 
		__t( "Protect registry io safe"), 

		__t( "Opened" ), 
		__t( "Closed" ), 

		__t( " length: %u dump of data:\n" ), 

		__t( "last en string" )
}; 

#ifdef _DEBUG
ULONG all_strings_en_count = ARRAY_SIZE( all_strings_en ); 
#endif //_DEBUG
 