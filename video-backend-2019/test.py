import unittest
import subprocess


class AvcFramingTest(unittest.TestCase):
    def test__bear_framing(self):
        """Проверяет текстовую раскадровку для видео с мишкой."""
        def split_by_idr(framing):
            """Разбиваем раскадровку на гопы, генерируя число кадров в гопах."""
            gop_frames = 0
            for s in framing:
                if s == 'I':
                    yield gop_frames
                    gop_frames = 0
                gop_frames += 1

        avcframes = subprocess.run(['./avcframes', 'bear.h264'], stdout=subprocess.PIPE)

        # Должно быть 3 гопа длинами 30, 30 и 22
        gops = list(split_by_idr(avcframes.stdout))
        self.assertEqual(len(gops), 3)
        self.assertEqual(gops[0], 30)
        self.assertEqual(gops[1], 30)
        self.assertEqual(gops[2], 22)


if __name__ == '__main__':
    suite = unittest.TestSuite()
    suite.addTest(unittest.makeSuite(AvcFramingTest))

    runner = unittest.TextTestRunner(verbosity=2)
    runner.run(suite)
