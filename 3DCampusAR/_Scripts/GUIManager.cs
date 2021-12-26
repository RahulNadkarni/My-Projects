using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GUIManager : MonoBehaviour
{
    public static GUIManager Instance;

    //screens
    public List<GameObject> objScreens = new List<GameObject>();
    private Dictionary<string, GameObject> dicScreens = new Dictionary<string, GameObject>();

    private Stack<ScreenName> screenHistory = new Stack<ScreenName>();
    ScreenName currentScreen = ScreenName.Panel_Login;
    public CanvasScaler canvasScaler;

    private void Awake()
    {
        PlayerPrefs.DeleteAll();
        Instance = this;

        // to set resolution according to device type
        if (SystemInfo.deviceModel.Contains("iPad"))
        {
            canvasScaler.matchWidthOrHeight = 1;
        }
        else
        {
            canvasScaler.matchWidthOrHeight = 0;
        }

        //dictionary of screen
        // key = Panel_Login
        //value = Panel_Login.gameObject
        for (int i = 0; i < objScreens.Count; i++)
        {
            dicScreens.Add(objScreens[i].name, objScreens[i]);
        }
    }

    void Start()
    {
        // check is already login
        if (PlayerPrefs.GetInt("IsLogin") == 0)
        {
            ScreenController(ScreenName.Panel_Login);
        }
        else
        {
            ScreenController(ScreenName.Panel_Home);
        }
    }

    //load 3DWalkThrough scene
    public void Btn3D()
    {
        UnityEngine.SceneManagement.SceneManager.LoadScene("3DWalkThrough");
    }

    //load AR scene
    public void BtnAR()
    {
        UnityEngine.SceneManagement.SceneManager.LoadScene("ARCampus");
    }

    // enable registration screen from login panel
    public void BtnSignUp()
    {
        ScreenController(ScreenName.Panel_Register);

        //for undestanding
        //objScreens[0].SetActive(false);
        //objScreens[1].SetActive(true);
    }

    //enable login screen from registration screen
    public void BtnSignIn()
    {
        ScreenController(ScreenName.Panel_Login);
    }   

    // logic to activate and de activate screens
    public void ScreenController(ScreenName str)
    {
        if (screenHistory.Contains(currentScreen))
            screenHistory.Pop();
        else
            screenHistory.Push(str);

        dicScreens["" + currentScreen].SetActive(false);

        dicScreens["" + str].SetActive(true);
        currentScreen = str;
    }

    // button to display privacy policy
    public void BtnPrivacyPolicy()
    {
        Application.OpenURL("https://app.termly.io/document/privacy-policy/8374270e-29f4-4963-bbc2-2a3774c43f98");
    }
}