![stopmehook](https://github.com/user-attachments/assets/efb0d927-b217-4d6a-b269-603aeea191b6)
# Stop Me Hook 

## Introduction

Stop Me Hook is a hook programmed in C for the Xahau blockchain. A hook is a series of rules that enable smart logic in Xahau, considered the smart contracts of Xahau.

Stop Me Hook is a simple idea for others to use to create more sophisticated and customizable hooks.

## Explanation


Attention: This hook is educational and simplistic, due to the nature of Xahau, there are several methods to make this hook unusable, such as uninstalling the hook. Only use if you are sure of what you are doing on Mainnet. You could put your funds at risk.

Stop Me Hook allows you to assign an account that will decide whether or not you want the account on which the hook is enabled to send outgoing payments. This could be useful for accounts where we don't want payments to be made by mistake, savings accounts or accounts of people like children or elderly people that we don't want to make payments by mistake or only allow these payments at times we want, like weekends, end of month, etc...

The hook is installed in the account that we will call hook account. This account will be the one affected by the Stop Me Hook logic. To make it work we first need to decide the address that will be from now on responsible for the hook account (the account where Stop Me Hook is installed) to be able to send payments or not.

In order to assign an account which we will call “backup” we will use an Invoke transaction from the hook account. We will add the BACK parameter with the address of the backup account. We must translate it to HEX, for this we go to https://transia-rnd.github.io/xrpl-hex-visualizer/ , add the account and click “From Hex” and copy the value xrpAddress in our transaction.

If everything went well from now on the new commands we will give to the hook will be from the backup account. If we give it orders from the hook account, it would not make sense to install the hook if we are going to be able to activate and deactivate it at will.

To activate the hook account payment sending block we must activate the ACTI flag. To do this we will create an Invoke transaction with origin the backup account and destination the hook account. We will add the parameter ACTI and the value 01. If everything went well, from now on if we try to make payments from our payment account it will not be possible until we deactivate the ACTI flag.

To deactivate the ACTI flag and allow outgoing payments again, we create an Invoke transaction with source the backup account and destination the hook account. As hook parameter we will add ACTI and the value 00. If everything went well, from now on, the account will be able to operate normally again.

If we want to change the backup account for another account, we create an Invoke transaction with source the backup account and destination the hook account. We will use the Hook BACK parameter and the address of the new backup address in the same way we defined it last time.

If we want to completely eliminate the administration of the backup account and that the hook account no longer has a manager, we create an Invoke operation with origin the backup account and destination the hook account. As Hook parameter we will add DELE and as value the account translated to HEX where the hook is installed.


