![stopmehook](https://github.com/user-attachments/assets/efb0d927-b217-4d6a-b269-603aeea191b6)
# Stop Me Hook 

## Introduction

Stop Me Hook is a hook programmed in C for the Xahau blockchain. A hook is a series of rules that enable smart logic in Xahau, considered the smart contracts of Xahau.

Stop Me Hook is a simple idea for others to use to create more sophisticated and customizable hooks.

## Explanation


**Attention:** This hook is educational and simplistic, due to the nature of Xahau, there are several methods to make this hook unusable, such as uninstalling the hook. Only use if you are sure of what you are doing on Testnet/Mainnet. You could put your funds at risk. It is recommended to install on new accounts.

Stop Me Hook allows you to assign an account that will decide whether or not you want the account on which the hook is enabled to send outgoing payments. This could be useful for accounts where we don't want payments to be made by mistake, savings accounts or accounts of people like children or elderly people that we don't want to make payments by mistake or only allow these payments at times we want, like weekends, end of month, etc...

The hook is installed in the account that we will call hook account. This account will be the one affected by the Stop Me Hook logic. To make it work we first need to decide the address that will be from now on responsible for the hook account (the account where Stop Me Hook is installed) to be able to send payments or not.

In order to assign an account which we will call “backup” we will use an Invoke transaction from the hook account. We will add the BACK parameter with the address of the backup account. We must translate it to HEX, for this we go to https://transia-rnd.github.io/xrpl-hex-visualizer/ , add the account and click “From Hex” and copy the value xrpAddress in our transaction.

If everything went well from now on the new commands we will give to the hook will be from the backup account. If we give it orders from the hook account, it would not make sense to install the hook if we are going to be able to activate and deactivate it at will.

To activate the hook account payment sending block we must activate the ACTI flag. To do this we will create an Invoke transaction with origin the backup account and destination the hook account. We will add the parameter ACTI and the value 01. If everything went well, from now on if we try to make payments from our payment account it will not be possible until we deactivate the ACTI flag.

To deactivate the ACTI flag and allow outgoing payments again, we create an Invoke transaction with source the backup account and destination the hook account. As hook parameter we will add ACTI and the value 00. If everything went well, from now on, the account will be able to operate normally again.

If we want to change the backup account for another account, we create an Invoke transaction with source the backup account and destination the hook account. We will use the Hook BACK parameter and the address of the new backup address in the same way we defined it last time.

If we want to completely eliminate the administration of the backup account and that the hook account no longer has a manager, we create an Invoke operation with origin the backup account and destination the hook account. As Hook parameter we will add DELE and as value the account translated to HEX where the hook is installed.


## Testnet Installation and usage

HookHash: E31E3F125C377E89ACC76610DD017FBEE434FB5BDAE86DEF3C345B994916C64F

## Mainnet Installation and usage

1. You can do it by (XRPLWin Hook Install Tool)[https://xahau-testnet.xrplwin.com/tools/hook/from-hash]

2. Or you can do it sending the transaction below:

HookOn is activated to trigger for Invoke and Payment transactions. You can verify it copying the HookOn value (0xfffffffffffffffffffffffffffffffffffffff7ffffffffffffffffffbffffe) in this website: https://richardah.github.io/xrpl-hookon-calculator/

    const prepared = {
      "TransactionType": "SetHook",
      "Account": your_account_address,
      "Flags": 0,
      "Hooks": [
        {
          "Hook": {
            "HookHash": "1A9D1EEA98A9BE3C45A35872E51E36B6E73CBB7033A96CE0D98DB484215E0494",
            "HookNamespace": "0000000000000000000000000000000000000000000000000000000000000000",
            "HookOn": "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFFFFFFFFFFFFFBFFFFE",
          }
        }
      ],
      ...networkInfo.txValues,
    };

## How to uninstall the Stop Me Hook on Mainnet/Testnet?

    const prepared = {
      "TransactionType": "SetHook",
      "Account": your_account_address,
      "Flags": 0,
      Hooks:
    [        
        {                        
            Hook: {
                CreateCode: "",
                Flags: 1,
            }
        }
    ],
      ...networkInfo.txValues,
    };

## How to install the Stop Me Hook on Mainnet?

Same as Testnet but changing the hookhash. The Hookhash is B5822426DFE897EF446BBCDEC3F83D803EC80188390DDA74112F6AAFE202A9E8.

1. You can do it by [XRPLWin Hook Install Tool](https://xahau.xrplwin.com/tools/hook/from-hash)

2. Or you can do it sending the transaction below:

```
    const prepared = {
      "TransactionType": "SetHook",
      "Account": your_account_address,
      "Flags": 0,
      "Hooks": [
        {
          "Hook": {
            "HookHash": "B5822426DFE897EF446BBCDEC3F83D803EC80188390DDA74112F6AAFE202A9E8",
            "HookNamespace": "0000000000000000000000000000000000000000000000000000000000000000",
            "HookOn": "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFFFFFFFFFFFFFBFFFFE",
          }
        }
      ],
      ...networkInfo.txValues,
    };
```

## Ideas for your hooks

This is just a small example so that others can build similar ideas or extend the current hook. Some ideas I propose:

1. Use timestamps and ledger sequences to make the denial of payments happen for a given time and not for a flag.
2. Let several accounts be administrators of the management of the hook account instead of having only one backup account.
3. That the blocking of payments is from a certain amount.
4. That in case of detecting a strange situation, all the money is sent to the backup account or to another account for security.
