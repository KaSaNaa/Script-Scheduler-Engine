from setuptools import setup, find_packages

setup(
    name="ScriptScheduler",
    version="1.0",
    packages=find_packages(where="src"),
    package_dir={"": "src"},
    install_requires=[
        # Add your dependencies here
    ],
    entry_points={
        "console_scripts": [
            "scriptscheduler=scheduler:main",
        ],
    },
)