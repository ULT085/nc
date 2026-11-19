#!/usr/bin/env python3
"""把講稿（Markdown/純文字）轉成單一 mp3。

用法: python3 podcast/tts.py script.md episode.mp3 [--voice zh-TW-HsiaoChenNeural] [--rate -8%]
- 以空行分段、每段 <= 800 字送出合成，最後用 ffmpeg 串接。
- 以 '#' 開頭的標題行、程式碼區塊 ``` 會被略過（講稿應以口語描述程式碼）。
- 會自動使用 HTTPS_PROXY（雲端環境需要）。
"""
import argparse, asyncio, os, re, subprocess, tempfile

import edge_tts


def chunks(text, limit=800):
    text = re.sub(r"```.*?```", "", text, flags=re.S)
    paras = []
    for para in re.split(r"\n\s*\n", text):
        lines = [l.strip() for l in para.splitlines() if l.strip() and not l.lstrip().startswith("#")]
        para = re.sub(r"[*_`>]", "", "".join(lines)).strip()
        if para:
            paras.append(para)
    buf = ""
    for para in paras:
        if buf and len(buf) + len(para) > limit:
            yield buf
            buf = ""
        buf += para + "\n"
    if buf:
        yield buf


async def synth(text, path, voice, rate, proxy):
    for attempt in range(4):
        try:
            await edge_tts.Communicate(text, voice, rate=rate, proxy=proxy).save(path)
            if os.path.getsize(path) > 0:
                return
        except Exception as e:  # noqa: BLE001
            print(f"retry {attempt + 1}: {e}")
            await asyncio.sleep(2 * (attempt + 1))
    raise RuntimeError("TTS failed")


async def main():
    p = argparse.ArgumentParser()
    p.add_argument("script")
    p.add_argument("out")
    p.add_argument("--voice", default="zh-TW-HsiaoChenNeural")
    p.add_argument("--rate", default="-8%")
    a = p.parse_args()
    proxy = os.environ.get("HTTPS_PROXY") or os.environ.get("https_proxy")
    with open(a.script, encoding="utf-8") as f:
        parts = list(chunks(f.read()))
    with tempfile.TemporaryDirectory() as d:
        paths = []
        for i, t in enumerate(parts):
            path = os.path.join(d, f"{i:03d}.mp3")
            await synth(t, path, a.voice, a.rate, proxy)
            paths.append(path)
            print(f"[{i + 1}/{len(parts)}] ok")
        lst = os.path.join(d, "list.txt")
        with open(lst, "w") as f:
            f.writelines(f"file '{x}'\n" for x in paths)
        subprocess.run(["ffmpeg", "-y", "-loglevel", "error", "-f", "concat", "-safe", "0",
                        "-i", lst, "-c", "copy", a.out], check=True)
    print("wrote", a.out)


if __name__ == "__main__":
    asyncio.run(main())
