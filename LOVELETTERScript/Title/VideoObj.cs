using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using UnityEngine.Video;

public class VideoObj : MonoBehaviour
{
    // VideoPlayer�R���|�[�l���g
    [SerializeField] private VideoPlayer _videoPlayer;

    // StreamingAssets�̓���t�@�C���ւ̃p�X
    [SerializeField] private string _streamingAssetsMoviePath;

    public void VideoStart()
    {
        // URL�w��
        _videoPlayer.source = VideoSource.Url;

        // StreamingAssets�t�H���_�z���̃p�X�̓����URL�Ƃ��Ďw�肷��
        _videoPlayer.url = Path.Combine(Application.streamingAssetsPath, _streamingAssetsMoviePath);

        // �Đ�
        _videoPlayer.Play();
    }
}
