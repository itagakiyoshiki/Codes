using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using UnityEngine.Video;

public class VideoObj : MonoBehaviour
{
    // VideoPlayerコンポーネント
    [SerializeField] private VideoPlayer _videoPlayer;

    // StreamingAssetsの動画ファイルへのパス
    [SerializeField] private string _streamingAssetsMoviePath;

    public void VideoStart()
    {
        // URL指定
        _videoPlayer.source = VideoSource.Url;

        // StreamingAssetsフォルダ配下のパスの動画をURLとして指定する
        _videoPlayer.url = Path.Combine(Application.streamingAssetsPath, _streamingAssetsMoviePath);

        // 再生
        _videoPlayer.Play();
    }
}
