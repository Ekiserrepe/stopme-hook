/**
 * Stopme Hook - Check Readme for instructions
 */
#include "hookapi.h"

int64_t hook(uint32_t reserved ) {
    // Check hook account
    unsigned char hook_accid[20];
    hook_account((uint32_t)hook_accid, 20);

    // Check destination of the original txn
    uint8_t account_field[20];
    int32_t account_field_len = otxn_field(SBUF(account_field), sfDestination);

    // Check the sender of the origin txn
    uint8_t acc_id[20];
    otxn_field(SBUF(acc_id), sfAccount);

    // To know the txn type
    int64_t tt = otxn_type();

    // Checking if hook_accid and account_field are the same
    int equal = 0;
    BUFFER_EQUAL(equal, hook_accid, account_field, 20);

    // Hooks Parameters
    uint8_t time_param[4] = {'C', 'O', 'O', 'L'};
    uint8_t backup_param[4] = {'B', 'A', 'C', 'K'};
    uint8_t activated_param[4] = {'A', 'C', 'T', 'I'};
    uint8_t delete_param[4] = {'D', 'E', 'L', 'E'};

    // Getting ACTI value from Namespace
    uint8_t activated_ns;
    state(SVAR(activated_ns), activated_param,4);

    // Getting ACTI from Hook Param
    uint8_t activated_hp;
    otxn_param(SVAR(activated_hp), SBUF(activated_param));


    // Getting backup account from namespace if exists
    uint8_t backup_account_ns[20] = {0x00U};
    state(SVAR(backup_account_ns), backup_param, 4);


    // Check if the sender of the invoke is the same as the backup account in the namespace
    int equal_backup = 0;
    BUFFER_EQUAL(equal_backup, backup_account_ns, acc_id, 20);

    // Getting backup account from Hook param if exists
    uint8_t hookparam_account[20] = {0x00U};
    otxn_param(hookparam_account, 20, SBUF(backup_param));

    // Getting DELE Hook Param
    uint8_t delete_account[20] = {0x00U};
    otxn_param(delete_account, 20, SBUF(delete_param));

    // Comparing Hook Account and Delete Account, if they are the same, Hook allows you to delete your backup account from namespace and free your hook account.
    int equal_delete = 0;
    BUFFER_EQUAL(equal_delete, hook_accid, delete_account, 20);

    // If i want to write for first time the backup_account in the namesapace, I need an invoke from the hooks account, no previous record of an backup account in the namespace and the account of the backup account in the hook parameter
    if (!equal && tt == 99 &&  state(SVAR(backup_account_ns), backup_param, 4) != 20 && otxn_param(hookparam_account, 20, SBUF(backup_param))==20){
        state_set(SVAR(hookparam_account), backup_param, 4);
        accept(SBUF("Stopme: Backup account selected."), 1);
    }

    // if I want to enable the ACTI flag to don't let hook account to send paymetns transactions, I check the Invoke comes from the backup account, the ACTI flag is still not 1 and the ACTI Hook Param has 1 as value
    if (equal_backup && tt == 99 && activated_ns != 0x01 && activated_hp==0x01){
        state_set(SVAR(activated_hp), activated_param, 4);
        accept(SBUF("Stopme: Activating transaction blocking."), 2);
    }
    //If I want to disable the ACTI flag, I need the backup account invoking it with ACTI and 00 as Hook param value and the ACTI flag in the namespace previously with value 1 (Enabled)
    if (equal_backup && tt == 99 &&  activated_ns==0x01 && otxn_param(SVAR(activated_hp), SBUF(activated_param))==1 && activated_hp==0x00){
        state_set(SVAR(activated_hp), activated_param, 4);
        accept(SBUF("Stopme: Disabling transaction blocking."), 3);
    }
    //If i want to rewrite after first time the backup_account, I need an invoke txn from the backup account registered in the namespace with BACK Hook param and the new account as value.
    if (equal_backup && tt == 99 && otxn_param(hookparam_account, 20, SBUF(backup_param))==20){
        state_set(SVAR(hookparam_account), backup_param, 4);
        accept(SBUF("Stopme: Rewriting backup account."), 4);
    }

    //If we want to stop linking the backup account and using the hook account as a normal account, We need to create an Invoke txn from backup account, adding the DELE Hook param and the hook account as value
    if(equal_backup && tt == 99 && equal_delete){
        state_set(0, 0, activated_param, 4);
        state_set(0, 0, backup_param, 4);
        accept(SBUF("Stopme: Deleting backup account and the activating flag."), 5);
    }
    //If the Hook Account tries to send an aoutgoing payment, we block it if there is a backup account in the namespace and the ACTI flag is enabled (=1).
    if(!equal && activated_ns == 0x01 && state(SVAR(backup_account_ns), backup_param, 4) == 20 && tt == 00){
        rollback(SBUF("Stopme: You can't transact until the backup account lets you or the backup account deletes itself as your manager."), 6);
    }
    accept(SBUF("Stopme: Nothing happened."), 7);
    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}