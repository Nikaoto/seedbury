package ge.seedbury.game;

import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.telephony.TelephonyManager;
import android.widget.Toast;

import org.cocos2dx.cpp.AppActivity;

/**
 * Created by Nika on 7/9/2018.
 * Check's the device's network connectivity and speed
 */
public class Connectivity {

    public static NetworkInfo getNetworkInfo(Context context) {
        ConnectivityManager cm = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        if (cm != null) {
            return cm.getActiveNetworkInfo();
        } else {
            return null;
        }
    }


    public static void toastMessage(final String message) {
        ((Activity) (AppActivity.getContext())).runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(AppActivity.getContext(), message, Toast.LENGTH_LONG).show();
            }
        });
    }


    /**
     * Check if there is any connectivity
     * @param context
     * @return
     */
    public static boolean isConnected(Context context) {
        NetworkInfo info = Connectivity.getNetworkInfo(context);
        return info != null && info.isConnected();
    }

    /**
     * Check if connection is through mobile carrier
     * @param context
     * @return
     */
    public static boolean isConnectedMobile(Context context){
        NetworkInfo info = Connectivity.getNetworkInfo(context);
        return info != null && info.isConnected() && info.getType() == ConnectivityManager.TYPE_MOBILE;
    }

    /**
     * Check if connection is through wifi
     * @param context
     * @return
     */
    public static boolean isConnectedWifi(Context context) {
        NetworkInfo info = Connectivity.getNetworkInfo(context);
        return info != null && info.isConnected() && info.getType() == ConnectivityManager.TYPE_WIFI;
    }


    /**
     * Check if connection is fast
     * @param context
     * @return
     */
    public static boolean isConnectedFast(Context context) {
        NetworkInfo info = Connectivity.getNetworkInfo(context);
        return info != null && info.isConnected() && Connectivity.isConnectionFast(info.getType(), info.getSubtype());
    }

    /**
     * Used for checking connection speed (in isConnectedFast)
     * @param networkInfoType
     * @param networkInfoSubtype
     * @return
     */
    private static boolean isConnectionFast(int networkInfoType, int networkInfoSubtype) {
        if (networkInfoType == ConnectivityManager.TYPE_WIFI) {
            return true;
        } else if (networkInfoType == ConnectivityManager.TYPE_MOBILE) {
            switch(networkInfoSubtype) {
                case TelephonyManager.NETWORK_TYPE_HSUPA:
                    return true; // ~ 1-23 Mbps
                case TelephonyManager.NETWORK_TYPE_HSDPA:
                    return true; // ~ 2-14 Mbps
                case TelephonyManager.NETWORK_TYPE_UMTS:
                    return true; // ~ 400-7000 kbps
                case TelephonyManager.NETWORK_TYPE_HSPA:
                    return true; // ~ 700-1700 kbps
                case TelephonyManager.NETWORK_TYPE_EVDO_A:
                    return true; // ~ 600-1400 kbps
                case TelephonyManager.NETWORK_TYPE_EVDO_0:
                    return true; // ~ 400-1000 kbps
                case TelephonyManager.NETWORK_TYPE_GPRS:
                    return true; // ~ 100 kbps
                case TelephonyManager.NETWORK_TYPE_UNKNOWN:
                    return false;
                default:
                    return false;
            }
        } else {
            return false;
        }
    }


    // Methods exposed to JNI

    public static boolean isConnectedToInternet() {
        return isConnected(AppActivity.getContext());
    }

    public static boolean isConnectedToFastInternet() {
        return isConnectedFast(AppActivity.getContext());
    }

    public static boolean isConnectedToMobileInternet() {
        return isConnectedMobile(AppActivity.getContext());
    }

}
