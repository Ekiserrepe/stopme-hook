// README & more info: https://github.com/Ekiserrepe/stopme-hook

import {sfAccount, sfDestination} from "sfcodes.ts";
import {ttPAYMENT, ttINVOKE} from "tts.ts";

export const Hook = () => {
    // Check hook account
    const hook_accid= hook_account() as number[]

    // Check destination of the original txn
    const account_field = otxn_field(sfDestination) as number[]

    // Check the sender of the origin txn
    const acc_id=otxn_field(sfAccount) as number[]

    // To know the txn type
    const tt = otxn_type()

    // Checking if hook_accid and account_field are the same
    const equal = JSON.stringify(hook_accid) == JSON.stringify(account_field) ? 1 : 0;

    // Hooks Parameters (It's needed to translate the string to hex string https://transia-rnd.github.io/xrpl-hex-visualizer/)

    //BACK is 4241434B
    const backup_param = [0x42, 0x41, 0x43, 0x4B]
    //ACTI is 41435449
    const activated_param = [0x41, 0x43, 0x54, 0x49]
    //DELE is 44454C45
    const delete_param = [0x44, 0x45, 0x4C, 0x45]

    // Getting ACTI value from Namespace
    const activated_ns=state(activated_param)

    // Getting ACTI from Hook Param
    const activated_hp=otxn_param(activated_param)

    // Getting backup account from namespace if exists
    const backup_account_ns=state(backup_param)

    // Check if the sender of the invoke is the same as the backup account in the namespace
    const equal_backup = JSON.stringify(backup_account_ns) == JSON.stringify(acc_id) ? 1 : 0;

    // Getting backup account from Hook param if exists
    const hookparam_account=otxn_param(backup_param)

    // Getting DELE Hook Param
    const delete_account=otxn_param(delete_param)

    // Comparing Hook Account and Delete Account, if they are the same, Hook allows you to delete your backup account from namespace and free your hook account.
    const equal_delete = JSON.stringify(hook_accid) == JSON.stringify(delete_account) ? 1 : 0;

    // If i want to write for first time the backup_account in the namesapace, I need an invoke from the hooks account, no previous record of an backup account in the namespace and the account of the backup account in the hook parameter
    if (!equal && tt == ttINVOKE &&  backup_account_ns.length != 20 && hookparam_account.length == 20){
        state_set(hookparam_account, backup_param)
        accept("Stopme: Backup account selected.", 1)
    }
    // if I want to enable the ACTI flag to don't let hook account to send paymetns transactions, I check the Invoke comes from the backup account, the ACTI flag is still not 1 and the ACTI Hook Param has 1 as value
    if (equal_backup && tt == ttINVOKE && activated_ns!=0x01 && activated_hp==0x01){
        state_set(activated_hp, activated_param)
        accept("Stopme: Activating transaction blocking.", 2)
    }
    //If I want to disable the ACTI flag, I need the backup account invoking it with ACTI and 00 as Hook param value and the ACTI flag in the namespace previously with value 1 (Enabled)
    if (equal_backup && tt == ttINVOKE &&  activated_ns==0x01 && activated_hp==0x00){
        state_set(activated_hp, activated_param)
        accept("Stopme: Disabling transaction blocking.", 3)
    }
    //If i want to rewrite after first time the backup_account, I need an invoke txn from the backup account registered in the namespace with BACK Hook param and the new account as value.
    if (equal_backup && tt == ttINVOKE && hookparam_account.length==20){
        state_set(hookparam_account, backup_param)
        accept("Stopme: Rewriting backup account.", 4)
    }
    //If we want to stop linking the backup account and using the hook account as a normal account, We need to create an Invoke txn from backup account, adding the DELE Hook param and the hook account as value
    if(equal_backup && tt == ttINVOKE && equal_delete){
        state_set(null, activated_param)
        state_set(null, backup_param)
        accept("Stopme: Deleting backup account and the activating flag.", 5)
    }  
    //If the Hook Account tries to send an aoutgoing payment, we block it if there is a backup account in the namespace and the ACTI flag is enabled (=1).
    if(!equal && activated_ns == 0x01 && backup_account_ns.length === 20 && tt == ttPAYMENT){
        rollback("Stopme: You can't transact until the backup account lets you or the backup account deletes itself as your manager.", 6)
    }
    accept("Stopme: Nothing happened.", 7)
    return 0
}