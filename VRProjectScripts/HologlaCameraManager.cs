﻿using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.SceneManagement;
using UnityEngine.XR.ARFoundation;

namespace Hologla
{
    public class HologlaCameraManager : MonoBehaviour
    {

        //表示モード(AR/MR/VR).
        public enum ViewMode
        {
            AR,
            MR,
            VR,
        };
        //1眼/2眼モード.
        public enum EyeMode
        {
            SingleEye,
            TwoEyes,
        }
        //画面表示サイズ.
        public enum ViewSize
        {
            Size1,
            Size2,
            Size3,
            Size4,
            Size5,
            Size6,
        };

        //表示モード(AR/MR/VR).
        [SerializeField] private ViewMode currentViewMode = ViewMode.MR;
        public ViewMode CurrentViewMode => currentViewMode;
        //1眼/2眼モード.
        [SerializeField] private EyeMode currentEyeMode = EyeMode.TwoEyes;
        public EyeMode CurrentEyeMode => currentEyeMode;
        //VRモード時以外に非表示にする背景用レイヤー.
        [SerializeField] private LayerMask vrBackgroundLayer = 0;
        //VRモード時以外に非表示にするオブジェクトリスト.
        [SerializeField] private List<GameObject> vrBackgroundObjList = new List<GameObject>();
        //VRモード時用のクリアフラグ.
        [SerializeField] private CameraClearFlags vrClearFlags = CameraClearFlags.Skybox;
        //瞳孔間距離(mm単位).
        [SerializeField] private float interpupillaryDistance = 64.0f;
        public float CurrentinterpupillaryDistance => interpupillaryDistance;
        //AR/MR時に使用する現実空間のコリジョン用オブジェクトリスト.
        [SerializeField] private List<GameObject> arMrCollisionObjList = new List<GameObject>();
        //表示領域サイズ(スマホサイズ対応用).
        [SerializeField] private ViewSize currentViewSize = ViewSize.Size1;
        public ViewSize CurrentViewSize => currentViewSize;

        //NearClip.
        [SerializeField] private float nearClippingPlane = 0.3f;
        [SerializeField] private float farClippingPlane = 1000.0f;
        #region
        public float nearClipPlane
        {
            get
            {
                return nearClippingPlane;
            }
            set
            {
                nearClippingPlane = value;
                ApplyClippingPlanes();
            }
        }
        public float farClipPlane
        {
            get
            {
                return farClippingPlane;
            }
            set
            {
                nearClippingPlane = value;
                ApplyClippingPlanes();
            }
        }
        #endregion

        [SerializeField] private Camera singleCamera = null;
        [SerializeField] private Camera leftEyeCamera = null;
        [SerializeField] private Camera rightEyeCamera = null;
        [SerializeField] private Canvas singleEyeFrameCanvas = null;
        [SerializeField] private Canvas leftEyeFrameCanvas = null;
        [SerializeField] private Canvas rightEyeFrameCanvas = null;

        public Material arBackgroundMaterial = null;

        private ViewMode prevViewMode = ViewMode.MR;
        private EyeMode prevEyeMode = EyeMode.TwoEyes;
        private float prevInterpupillaryDistance = 64.0f;
        private ViewSize prevViewSize = ViewSize.Size1;

        public float InterpupillaryDistance { get { return interpupillaryDistance; } }

        private const float SCREEN_FRAME_NEAR_CLIPPING_OFFSET = 1.0f;

        [SerializeField] private ARCameraBackground[] arCameraBackgroundArray = null;
#if UNITY_ANDROID && false
		private GoogleARCore.ARCoreBackgroundRenderer singleArCameraBackground = null ;
		private GoogleARCore.ARCoreBackgroundRenderer leftArCameraBackground = null ;
		private GoogleARCore.ARCoreBackgroundRenderer rightArCameraBackground = null ;
#elif UNITY_IOS && false
		private HologlaARKitVideo singleArCameraBackground = null ;
		private HologlaARKitVideo leftArCameraBackground = null ;
		private HologlaARKitVideo rightArCameraBackground = null ;
#endif

        // ARモード時に自動的にARオクルージョンを有効にするかどうか.
        [SerializeField] private bool isValidArOcclusion = false;

        [SerializeField] private List<AROcclusionManager> arOcclusionManagerList = new List<AROcclusionManager>();

        // Use this for initialization
        void Start()
        {
            //一部Awakeだと適用されない設定があるので、Start側でも再設定しておく.
            SwitchViewMode(currentViewMode);
            SwitchEyeMode(currentEyeMode);
            ApplyIPD(interpupillaryDistance);
            SwitchViewSize(currentViewSize);

            UpdateArOcclution();

            return;
        }

        // Update is called once per frame
        void Update()
        {
            return;
        }

        private void Awake()
        {
            //デフォルトで自動スリープを無効化しておく.
            Screen.sleepTimeout = SleepTimeout.NeverSleep;

            //子となるカメラを一度全て無効にしておく.
            Camera[] childCameraArray;

            childCameraArray = GetComponentsInChildren<Camera>();
            foreach (Camera camera in childCameraArray)
            {
                if (gameObject != camera.gameObject)
                {
                    camera.enabled = false;
                }
            }
            //ARカメラ用に紐づけるARKit/ARCore用コンポーネントを設定する.
#if UNITY_ANDROID
            if (null != arBackgroundMaterial)
            {
                singleCamera.gameObject.SetActive(true);
                leftEyeCamera.gameObject.SetActive(true);
                rightEyeCamera.gameObject.SetActive(true);
            }
#elif UNITY_IOS
			if( null != arBackgroundMaterial ){
				singleCamera.gameObject.SetActive(true);
				leftEyeCamera.gameObject.SetActive(true);
				rightEyeCamera.gameObject.SetActive(true);
			}
#endif
            if (null == arCameraBackgroundArray)
            {
                arCameraBackgroundArray = GetComponentsInChildren<ARCameraBackground>(true);
            }
            foreach (ARCameraBackground arCameraBackground in arCameraBackgroundArray)
            {
                arCameraBackground.useCustomMaterial = true;
                arCameraBackground.customMaterial = arBackgroundMaterial;
            }

            currentViewMode = UserSettings.viewMode;
            currentEyeMode = UserSettings.eyeMode;
            interpupillaryDistance = UserSettings.interpupillaryDistance;
            currentViewSize = UserSettings.viewSize;

            SwitchViewMode(currentViewMode);
            SwitchEyeMode(currentEyeMode);
            ApplyIPD(interpupillaryDistance);
            SwitchViewSize(currentViewSize);

#if UNITY_EDITOR
            if (null != transform.parent.gameObject && null == transform.parent.gameObject.GetComponent<EditorCameraController>())
            {
                transform.parent.gameObject.AddComponent<EditorCameraController>();
            }
#endif
            UpdateArOcclution();

            return;
        }

        private void OnValidate()
        {
            if (prevViewMode != currentViewMode)
            {
                SwitchViewMode(currentViewMode);
            }
            if (prevEyeMode != currentEyeMode)
            {
                SwitchEyeMode(currentEyeMode);
            }
            if (prevInterpupillaryDistance != interpupillaryDistance)
            {
                ApplyIPD(interpupillaryDistance);
            }
            if (prevViewSize != currentViewSize)
            {
                SwitchViewSize(currentViewSize);
            }
            ApplyClippingPlanes();

            return;
        }

        //AR/MR/VRの切り替え.
        public void SwitchViewMode(ViewMode viewMode)
        {
            if (ViewMode.AR == viewMode)
            {
                SwitchBackgroundVisible(false);
                SwitchARCameraValid(true);
                SwitchRealCollisionValid(true);
            }
            else if (ViewMode.MR == viewMode)
            {
                SwitchBackgroundVisible(false);
                SwitchARCameraValid(false);
                SwitchRealCollisionValid(true);
            }
            else if (ViewMode.VR == viewMode)
            {
                SwitchBackgroundVisible(true);
                SwitchARCameraValid(false);
                SwitchRealCollisionValid(false);
            }

            prevViewMode = currentViewMode;
            currentViewMode = viewMode;
            // ViewModeが切り替わった場合はARオクルージョンの常態も更新しておく.
            UpdateArOcclution();

            return;
        }
        #region
        //AR/MR/VRの切り替え.
        public void SwitchViewModeAR() { SwitchViewMode(ViewMode.AR); }
        public void SwitchViewModeMR() { SwitchViewMode(ViewMode.MR); }
        public void SwitchViewModeVR() { SwitchViewMode(ViewMode.VR); }
        #endregion

        //1眼/2眼の切り替え.
        public void SwitchEyeMode(EyeMode eyeMode)
        {
            if (EyeMode.TwoEyes == eyeMode)
            {
                singleCamera.enabled = false;
                leftEyeCamera.enabled = true;
                rightEyeCamera.enabled = true;
            }
            else
            {
                singleCamera.enabled = true;
                leftEyeCamera.enabled = false;
                rightEyeCamera.enabled = false;
            }
            prevEyeMode = currentEyeMode;
            currentEyeMode = eyeMode;

            return;
        }
        #region
        //1眼/2眼の切り替え.
        public void SwitchEyeModeSingle() { SwitchEyeMode(EyeMode.SingleEye); }
        public void SwitchEyeModeTwo() { SwitchEyeMode(EyeMode.TwoEyes); }
        #endregion

        //瞳孔間距離の設定.
        public void ApplyIPD(float ipd)
        {
            float metor2milli = 0.001f;

            leftEyeCamera.transform.localPosition = Vector3.left * (ipd * 0.5f) * metor2milli;
            rightEyeCamera.transform.localPosition = Vector3.right * (ipd * 0.5f) * metor2milli;

            prevInterpupillaryDistance = interpupillaryDistance;
            interpupillaryDistance = ipd;

            return;
        }
        //瞳孔間距離の設定.
        public void AddIPD(float addValue)
        {
            ApplyIPD(interpupillaryDistance + addValue);

            return;
        }

        //表示領域サイズの切り替え.
        public void SwitchViewSize(ViewSize viewSize)
        {
            Vector2 useViewportSize;

            useViewportSize = UserSettings.viewportSizeList[(int)viewSize];

            singleCamera.rect = new Rect((0.5f - useViewportSize.x), 0.0f, useViewportSize.x * 2, useViewportSize.y);
            leftEyeCamera.rect = new Rect(0.5f - useViewportSize.x, 0.0f, useViewportSize.x - 0.01f, useViewportSize.y);
            rightEyeCamera.rect = new Rect(0.5f + 0.01f, 0.0f, useViewportSize.x - 0.01f, useViewportSize.y);

            prevViewSize = currentViewSize;
            currentViewSize = viewSize;

            return;
        }
        #region
        //表示領域サイズの切り替え.
        public void SwitchViewSize1() { SwitchViewSize(ViewSize.Size1); }
        public void SwitchViewSize2() { SwitchViewSize(ViewSize.Size2); }
        public void SwitchViewSize3() { SwitchViewSize(ViewSize.Size3); }
        public void SwitchViewSize4() { SwitchViewSize(ViewSize.Size4); }
        public void SwitchViewSize5() { SwitchViewSize(ViewSize.Size5); }
        public void SwitchViewSize6() { SwitchViewSize(ViewSize.Size6); }
        #endregion

        //VR時に背景として扱うオブジェクトを追加する.
        public void AddVrBackgroundObj(GameObject backgroundObj)
        {
            vrBackgroundObjList.Add(backgroundObj);

            return;
        }
        //VR時に背景として扱うオブジェクトリストを空にする.
        public void ClearVrBackgroundObj()
        {
            vrBackgroundObjList.Clear();

            return;
        }
        //VR時に背景として扱うオブジェクトリストから指定オブジェクトを削除する.
        public void RemoveVrBackgroundObj(GameObject backgroundObj)
        {
            if (true == vrBackgroundObjList.Contains(backgroundObj))
            {
                vrBackgroundObjList.Remove(backgroundObj);
            }

            return;
        }

        private void SwitchBackgroundVisible(bool isVisible)
        {
            if (false == isVisible)
            {
                //背景レイヤーを非表示化.
                singleCamera.cullingMask = LayerMask.NameToLayer("Everything") ^ vrBackgroundLayer;
                leftEyeCamera.cullingMask = LayerMask.NameToLayer("Everything") ^ vrBackgroundLayer;
                rightEyeCamera.cullingMask = LayerMask.NameToLayer("Everything") ^ vrBackgroundLayer;
                //クリアフラグを変更.
                singleCamera.clearFlags = CameraClearFlags.Skybox;
                singleCamera.backgroundColor = Color.black;
                leftEyeCamera.clearFlags = CameraClearFlags.Skybox;
                leftEyeCamera.backgroundColor = Color.black;
                rightEyeCamera.clearFlags = CameraClearFlags.Skybox;
                rightEyeCamera.backgroundColor = Color.black;
            }
            else
            {
                //背景レイヤーを表示.
                singleCamera.cullingMask = LayerMask.NameToLayer("Everything");
                leftEyeCamera.cullingMask = LayerMask.NameToLayer("Everything"); ;
                rightEyeCamera.cullingMask = LayerMask.NameToLayer("Everything"); ;
                //クリアフラグを変更.
                singleCamera.clearFlags = vrClearFlags;
                leftEyeCamera.clearFlags = vrClearFlags;
                rightEyeCamera.clearFlags = vrClearFlags;
            }
            foreach (GameObject backgroundObj in vrBackgroundObjList)
            {
                if (null != backgroundObj)
                {
                    backgroundObj.SetActive(isVisible);
                }
            }

            return;
        }

        //現在の各種設定を保存する.
        public void SaveCurrentSetting()
        {
            UserSettings.viewMode = currentViewMode;
            UserSettings.eyeMode = currentEyeMode;
            UserSettings.interpupillaryDistance = interpupillaryDistance;
            UserSettings.viewSize = currentViewSize;

            UserSettings.WriteSettings();

            return;
        }

        private void SwitchRealCollisionValid(bool isValid)
        {
            foreach (GameObject collisionObj in arMrCollisionObjList)
            {
                collisionObj.SetActive(isValid);
            }

            return;
        }

        private void SwitchARCameraValid(bool isValid)
        {
            if (null != arCameraBackgroundArray)
            {
                foreach (ARCameraBackground arCameraBackground in arCameraBackgroundArray)
                {
                    arCameraBackground.enabled = isValid;
                }
            }

            return;
        }

        private void ApplyClippingPlanes()
        {
            if (null != singleCamera)
            {
                singleCamera.nearClipPlane = nearClipPlane;
                singleCamera.farClipPlane = farClipPlane;
                if (null != singleEyeFrameCanvas)
                {
                    singleEyeFrameCanvas.planeDistance = nearClipPlane + SCREEN_FRAME_NEAR_CLIPPING_OFFSET;
                }
            }
            if (null != leftEyeCamera)
            {
                leftEyeCamera.nearClipPlane = nearClipPlane;
                leftEyeCamera.farClipPlane = farClipPlane;
                if (null != leftEyeFrameCanvas)
                {
                    leftEyeFrameCanvas.planeDistance = nearClipPlane + SCREEN_FRAME_NEAR_CLIPPING_OFFSET;
                }
            }
            if (null != rightEyeCamera)
            {
                rightEyeCamera.nearClipPlane = nearClipPlane;
                rightEyeCamera.farClipPlane = farClipPlane;
                if (null != rightEyeFrameCanvas)
                {
                    rightEyeFrameCanvas.planeDistance = nearClipPlane + SCREEN_FRAME_NEAR_CLIPPING_OFFSET;
                }
            }

            return;
        }

        // ARモード中にARオクルージョンを有効にするかどうかを切り替える.
        public void SwitchArOcclusion(bool isValid)
        {
            isValidArOcclusion = isValid;
            UpdateArOcclution();

            return;
        }

        private void UpdateArOcclution()
        {
            // ARオクルージョン用のコンポーネントが設定されておらず、ARオクルージョンを有効にしたい場合はコンポーネントを取得してくる.
            if (0 >= arOcclusionManagerList.Count)
            {
                foreach (ARCameraBackground arCameraBackground in arCameraBackgroundArray)
                {
                    if (null != arCameraBackground)
                    {
                        AROcclusionManager arOcclusionManager = arCameraBackground.gameObject.GetComponent<AROcclusionManager>();
                        if (null != arOcclusionManager)
                        {
                            arOcclusionManagerList.Add(arOcclusionManager);
                        }
                    }
                }
            }

            // ARオクルージョンの有効無効設定と、ARモード中かどうかを見て、ARオクルージョンの有効状態を切り替える.
            if (false == isValidArOcclusion || ViewMode.AR != currentViewMode)
            {
                foreach (AROcclusionManager arOcclusionManager in arOcclusionManagerList)
                {
                    if (null != arOcclusionManager && true == arOcclusionManager.enabled)
                    {
                        arOcclusionManager.enabled = false;
                    }
                }
            }
            else
            {
                foreach (AROcclusionManager arOcclusionManager in arOcclusionManagerList)
                {
                    if (null != arOcclusionManager && false == arOcclusionManager.enabled)
                    {
                        arOcclusionManager.enabled = true;
                    }
                }
            }

            return;
        }

        //====================シーン制御/prefab制御用====================.
        public void AddScene(string sceneName)
        {
            SceneManager.LoadScene(sceneName, LoadSceneMode.Additive);

            return;
        }

        public void SwitchScene(string sceneName)
        {
            SceneManager.LoadScene(sceneName, LoadSceneMode.Single);

            return;
        }

        public void LoadPrefab(GameObject prefab)
        {
            GameObject.Instantiate(prefab);

            return;
        }
        //====================シーン制御/prefab制御用====================.

    }


#if UNITY_EDITOR
    [UnityEditor.CustomEditor(typeof(HologlaCameraManager))]
    class HologlaCameraManager_Editor : UnityEditor.Editor
    {
        public override void OnInspectorGUI()
        {
            //元のInspector部分を表示
            base.OnInspectorGUI();

            //ボタンを表示
            if (true == GUILayout.Button("SaveCurrentSetting"))
            {
                HologlaCameraManager hologlaCameraManager = target as HologlaCameraManager;
                if (null != hologlaCameraManager)
                {
                    hologlaCameraManager.SaveCurrentSetting();
                }
            }
        }
    }
#endif

}
