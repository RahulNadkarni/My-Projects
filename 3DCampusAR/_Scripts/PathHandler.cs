using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;
using UnityEngine.UI;

public class PathHandler : MonoBehaviour
{

    public LineRenderer line; //to hold the line Renderer
    public Transform target; //to hold the transform of the target
    public NavMeshAgent agent;

    public List<Transform> pos = new List<Transform>();
    public Dropdown from, to;   

    void Start()
    {
        //StartCoroutine(getPath());
    }

    IEnumerator getPath()
    {
        agent.enabled = true;
        //agent.CalculatePath(target.position, navPath);//.SetDestination(target.position); //create the path
        NavMeshPath navPath = new NavMeshPath();

        NavMesh.CalculatePath(agent.transform.position, target.position, NavMesh.AllAreas, navPath);

        yield return new WaitForEndOfFrame(); //wait for the path to generate

        DrawPath(navPath);

        agent.isStopped = true;//add this if you don't want to move the agent
    }

    void DrawPath(NavMeshPath path)
    {
        if (path.corners.Length < 2) //if the path has 1 or no corners, there is no need
            return;

        line.positionCount = path.corners.Length; //set the array of positions to the amount of corners

        line.SetPosition(0, new Vector3(agent.transform.position.x, 0.2f, agent.transform.position.z));

        for (var i = 1; i < path.corners.Length; i++)
        {
            line.SetPosition(i, new Vector3(path.corners[i].x, 0.2f, path.corners[i].z)); //go through each corner and set that to the line renderer's position
//            Debug.Log(path.corners[i]);
        }
    }

    int index_To, index_From;

    public void OnDropDown_From(int i)
    {        
        index_From = i;

        if(index_To == index_From)
        {
            line.positionCount = 0;
        }

        agent.enabled = false;
        agent.transform.localPosition = pos[i].localPosition;
        StartCoroutine(getPath());
    }

    public void OnDropDown_To(int i)
    {
        index_To = i;

        if (index_To == index_From)
        {
            line.positionCount = 0;
        }

        target.localPosition = pos[i].localPosition;
        StartCoroutine(getPath());
    }
}