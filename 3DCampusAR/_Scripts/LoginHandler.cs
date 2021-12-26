using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.UI;
using System.Net;

public class LoginHandler : MonoBehaviour
{
    public static LoginHandler Instance;

    [System.Serializable]
    public class Response
    {
        public int status;
        public string message;
    }

    public List<InputField> inptLogin = new List<InputField>();
    public List<InputField> inptRegister = new List<InputField>();
    public GameObject panel_Loading,panel_Error;

    public Response response = new Response();
    public Text txtMessage;

    private void Awake()
    {
        Instance = this;
    }

    void Start()
    {
       
    }

    public void BtnSubmitSignUp()
    {

        if (CheckInput(inptRegister[0].text) || CheckInput(inptRegister[1].text) || CheckInput(inptRegister[2].text) || CheckInput(inptRegister[3].text) || !VerifyEmailAddress(inptRegister[1].text))
        {
            txtMessage.text = "Please enter the correct details";
            panel_Error.SetActive(true);
            return;
        }

        StartCoroutine(ExecuteAPI_Post("https://acusion.com/campusar/index.php/api/user/signup", inptRegister[0].text, inptRegister[1].text, inptRegister[2].text, "normal"));
    }

    public void BtnSubmitSignIn()
    {
        if (CheckInput(inptLogin[0].text) || CheckInput(inptLogin[1].text) || !VerifyEmailAddress(inptLogin[0].text))
        {
            txtMessage.text = "Please enter the correct details";
            panel_Error.SetActive(true);
            return;
        }

        StartCoroutine(ExecuteAPI_Post("https://acusion.com/campusar/index.php/api/user/login", "", inptLogin[0].text, inptLogin[1].text, "normal"));
    }

    bool CheckInput(string str)
    {
       return string.IsNullOrEmpty(str);
    }

    public IEnumerator ExecuteAPI_Post(string URL,string name,string email,string password,string type)
    {
        if (!HasConnection())
        {
            txtMessage.text = "No Internet connection";
            panel_Error.SetActive(true);

            yield break;
        }

        panel_Loading.SetActive(true);

        Debug.Log("ExecuteAPI_Post  "+name);
        WWWForm form = new WWWForm();
        form.AddField("name",name);
        form.AddField("email", email);
        form.AddField("password", password);
        form.AddField("social_type", type);
        form.AddField("social_id", email);

        using (UnityWebRequest www = UnityWebRequest.Post(URL, form))
        {
            www.method = "POST";            
            yield return www.SendWebRequest();

            panel_Loading.SetActive(false);

            if (www.isNetworkError || www.isHttpError || www.responseCode == 500 || www.responseCode == 404)
            {
                Debug.Log(www.error);
            }
            else
            {
                response = JsonUtility.FromJson<Response>(www.downloadHandler.text);

                if (response.status == 1)
                {
                    PlayerPrefs.SetInt("IsLogin", 1);
                    Debug.Log(www.downloadHandler.text);
                    GUIManager.Instance.ScreenController(ScreenName.Panel_Home);
                }
                else
                {
                    txtMessage.text = response.message;
                    panel_Error.SetActive(true);
                }
            }
        }
    }

    bool VerifyEmailAddress(string address)
    {
        string[] atCharacter;
        string[] dotCharacter;
        // this splits the input address by the @ symbol. If it finds the @ symbol, it throws it away,
        // and puts what comes before the @ symbol into atCharacter[0], and what comes after the @ symbol
        // into atCharacter[1]
        atCharacter = address.Split("@"[0]);
        //now we check that the returned array is exactly 2 members long, that means there was only 1 @ symbol
        if (atCharacter.Length == 2)
        {
            //here we split the second member returned above by the dot character, and shove the returned info
            // into the dotCharacter array.
            dotCharacter = atCharacter[1].Split("."[0]);
            // now we check the length of the dotCharacter array. If it is greater than or equal to 2, we know
            // we have at least one dot, maybe more than one.
            if (dotCharacter.Length >= 2)
            {
                // this last check makes sure that there is actual data after the last dot.
                if (dotCharacter[dotCharacter.Length - 1].Length == 0)
                {
                    // fail
                    return false;
                }
                else
                {
                    // if we get to here, you have a valid email address format
                    return true;
                }
            }
            else
            {
                // fail
                return false;
            }
        }
        else
        {
            // fail
            return false;
        }
    }

    //***************************** to check internet connections *****************************************
    public bool HasConnection()
    {
        try
        {
            using (var client = new WebClient())
            using (var stream = new WebClient().OpenRead("http://www.google.com"))
            {
                return true;
            }
        }
        catch
        {
            return false;
        }
    }
}