import React from 'react';
import { PROPERTY_KEYS_DOCKVIEW, createDockview, } from 'dockview-core';
import { ReactPanelContentPart } from './reactContentPart';
import { ReactPanelHeaderPart } from './reactHeaderPart';
import { usePortalsLifecycle } from '../react';
import { ReactWatermarkPart } from './reactWatermarkPart';
import { ReactHeaderActionsRendererPart } from './headerActionsRenderer';
function createGroupControlElement(component, store) {
    return component
        ? (groupPanel) => {
            return new ReactHeaderActionsRendererPart(component, store, groupPanel);
        }
        : undefined;
}
const DEFAULT_REACT_TAB = 'props.defaultTabComponent';
function extractCoreOptions(props) {
    const coreOptions = PROPERTY_KEYS_DOCKVIEW.reduce((obj, key) => {
        if (key in props) {
            obj[key] = props[key];
        }
        return obj;
    }, {});
    return coreOptions;
}
export const DockviewReact = React.forwardRef((props, ref) => {
    const domRef = React.useRef(null);
    const dockviewRef = React.useRef();
    const [portals, addPortal] = usePortalsLifecycle();
    React.useImperativeHandle(ref, () => domRef.current, []);
    const prevProps = React.useRef({});
    React.useEffect(() => {
        const changes = {};
        PROPERTY_KEYS_DOCKVIEW.forEach((propKey) => {
            const key = propKey;
            const propValue = props[key];
            if (key in props && propValue !== prevProps.current[key]) {
                changes[key] = propValue;
            }
        });
        if (dockviewRef.current) {
            dockviewRef.current.updateOptions(changes);
        }
        else {
            // not yet fully initialized
        }
        prevProps.current = props;
    }, PROPERTY_KEYS_DOCKVIEW.map((key) => props[key]));
    React.useEffect(() => {
        var _a;
        if (!domRef.current) {
            return;
        }
        const frameworkTabComponents = (_a = props.tabComponents) !== null && _a !== void 0 ? _a : {};
        if (props.defaultTabComponent) {
            frameworkTabComponents[DEFAULT_REACT_TAB] =
                props.defaultTabComponent;
        }
        const frameworkOptions = {
            createLeftHeaderActionComponent: createGroupControlElement(props.leftHeaderActionsComponent, { addPortal }),
            createRightHeaderActionComponent: createGroupControlElement(props.rightHeaderActionsComponent, { addPortal }),
            createPrefixHeaderActionComponent: createGroupControlElement(props.prefixHeaderActionsComponent, { addPortal }),
            createComponent: (options) => {
                return new ReactPanelContentPart(options.id, props.components[options.name], {
                    addPortal,
                });
            },
            createTabComponent(options) {
                return new ReactPanelHeaderPart(options.id, frameworkTabComponents[options.name], {
                    addPortal,
                });
            },
            createWatermarkComponent: props.watermarkComponent
                ? () => {
                    return new ReactWatermarkPart('watermark', props.watermarkComponent, {
                        addPortal,
                    });
                }
                : undefined,
            defaultTabComponent: props.defaultTabComponent
                ? DEFAULT_REACT_TAB
                : undefined,
        };
        const api = createDockview(domRef.current, Object.assign(Object.assign({}, extractCoreOptions(props)), frameworkOptions));
        const { clientWidth, clientHeight } = domRef.current;
        api.layout(clientWidth, clientHeight);
        if (props.onReady) {
            props.onReady({ api });
        }
        dockviewRef.current = api;
        return () => {
            dockviewRef.current = undefined;
            api.dispose();
        };
    }, []);
    React.useEffect(() => {
        if (!dockviewRef.current) {
            return () => {
                // noop
            };
        }
        const disposable = dockviewRef.current.onDidDrop((event) => {
            if (props.onDidDrop) {
                props.onDidDrop(event);
            }
        });
        return () => {
            disposable.dispose();
        };
    }, [props.onDidDrop]);
    React.useEffect(() => {
        if (!dockviewRef.current) {
            return () => {
                // noop
            };
        }
        const disposable = dockviewRef.current.onWillDrop((event) => {
            if (props.onWillDrop) {
                props.onWillDrop(event);
            }
        });
        return () => {
            disposable.dispose();
        };
    }, [props.onWillDrop]);
    React.useEffect(() => {
        if (!dockviewRef.current) {
            return;
        }
        dockviewRef.current.updateOptions({
            createComponent: (options) => {
                return new ReactPanelContentPart(options.id, props.components[options.name], {
                    addPortal,
                });
            },
        });
    }, [props.components]);
    React.useEffect(() => {
        var _a;
        if (!dockviewRef.current) {
            return;
        }
        const frameworkTabComponents = (_a = props.tabComponents) !== null && _a !== void 0 ? _a : {};
        if (props.defaultTabComponent) {
            frameworkTabComponents[DEFAULT_REACT_TAB] =
                props.defaultTabComponent;
        }
        dockviewRef.current.updateOptions({
            defaultTabComponent: props.defaultTabComponent
                ? DEFAULT_REACT_TAB
                : undefined,
            createTabComponent(options) {
                return new ReactPanelHeaderPart(options.id, frameworkTabComponents[options.name], {
                    addPortal,
                });
            },
        });
    }, [props.tabComponents, props.defaultTabComponent]);
    React.useEffect(() => {
        if (!dockviewRef.current) {
            return;
        }
        dockviewRef.current.updateOptions({
            createWatermarkComponent: props.watermarkComponent
                ? () => {
                    return new ReactWatermarkPart('watermark', props.watermarkComponent, {
                        addPortal,
                    });
                }
                : undefined,
        });
    }, [props.watermarkComponent]);
    React.useEffect(() => {
        if (!dockviewRef.current) {
            return;
        }
        dockviewRef.current.updateOptions({
            createRightHeaderActionComponent: createGroupControlElement(props.rightHeaderActionsComponent, { addPortal }),
        });
    }, [props.rightHeaderActionsComponent]);
    React.useEffect(() => {
        if (!dockviewRef.current) {
            return;
        }
        dockviewRef.current.updateOptions({
            createLeftHeaderActionComponent: createGroupControlElement(props.leftHeaderActionsComponent, { addPortal }),
        });
    }, [props.leftHeaderActionsComponent]);
    React.useEffect(() => {
        if (!dockviewRef.current) {
            return;
        }
        dockviewRef.current.updateOptions({
            createPrefixHeaderActionComponent: createGroupControlElement(props.prefixHeaderActionsComponent, { addPortal }),
        });
    }, [props.prefixHeaderActionsComponent]);
    return (React.createElement("div", { style: { height: '100%', width: '100%' }, ref: domRef }, portals));
});
DockviewReact.displayName = 'DockviewComponent';
