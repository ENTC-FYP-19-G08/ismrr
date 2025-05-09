from setuptools import setup

package_name = 'face_recognition'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='vwm',
    maintainer_email='vwm@todo.todo',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'detect_face = face_recognition.detect_face:main',
            'camera_feed = fake_camera.camera_feed:main',
            'trigger = fake_camera.trigger_detect:main'
        ],
    },
)
