import React from 'react';
import { createPaneview, PROPERTY_KEYS_PANEVIEW, } from 'dockview-core';
import { usePortalsLifecycle } from '../react';
import { PanePanelSection } from './view';
function extractCoreOptions(props) {
    const coreOptions = PROPERTY_KEYS_PANEVIEW.reduce((obj, key) => {
        if (key in props) {
            obj[key] = props[key];
        }
        return obj;
    }, {});
    return coreOptions;
}
export const PaneviewReact = React.forwardRef((props, ref) => {
    const domRef = React.useRef(null);
    const paneviewRef = React.useRef();
    const [portals, addPortal] = usePortalsLifecycle();
    React.useImperativeHandle(ref, () => domRef.current, []);
    const prevProps = React.useRef({});
    React.useEffect(() => {
        const changes = {};
        PROPERTY_KEYS_PANEVIEW.forEach((propKey) => {
            const key = propKey;
            const propValue = props[key];
            if (key in props && propValue !== prevProps.current[key]) {
                changes[key] = propValue;
            }
        });
        if (paneviewRef.current) {
            paneviewRef.current.updateOptions(changes);
        }
        else {
            // not yet fully initialized
        }
        prevProps.current = props;
    }, PROPERTY_KEYS_PANEVIEW.map((key) => props[key]));
    React.useEffect(() => {
        var _a;
        if (!domRef.current) {
            return () => {
                // noop
            };
        }
        const headerComponents = (_a = props.headerComponents) !== null && _a !== void 0 ? _a : {};
        const frameworkOptions = {
            createComponent: (options) => {
                return new PanePanelSection(options.id, props.components[options.name], { addPortal });
            },
            createHeaderComponent: (options) => {
                return new PanePanelSection(options.id, headerComponents[options.name], { addPortal });
            },
        };
        const api = createPaneview(domRef.current, Object.assign(Object.assign({}, extractCoreOptions(props)), frameworkOptions));
        const { clientWidth, clientHeight } = domRef.current;
        api.layout(clientWidth, clientHeight);
        if (props.onReady) {
            props.onReady({ api });
        }
        paneviewRef.current = api;
        return () => {
            paneviewRef.current = undefined;
            api.dispose();
        };
    }, []);
    React.useEffect(() => {
        if (!paneviewRef.current) {
            return;
        }
        paneviewRef.current.updateOptions({
            createComponent: (options) => {
                return new PanePanelSection(options.id, props.components[options.name], { addPortal });
            },
        });
    }, [props.components]);
    React.useEffect(() => {
        var _a;
        if (!paneviewRef.current) {
            return;
        }
        const headerComponents = (_a = props.headerComponents) !== null && _a !== void 0 ? _a : {};
        paneviewRef.current.updateOptions({
            createHeaderComponent: (options) => {
                return new PanePanelSection(options.id, headerComponents[options.name], { addPortal });
            },
        });
    }, [props.headerComponents]);
    React.useEffect(() => {
        if (!paneviewRef.current) {
            return () => {
                // noop
            };
        }
        const disposable = paneviewRef.current.onDidDrop((event) => {
            if (props.onDidDrop) {
                props.onDidDrop(event);
            }
        });
        return () => {
            disposable.dispose();
        };
    }, [props.onDidDrop]);
    return (React.createElement("div", { style: { height: '100%', width: '100%' }, ref: domRef }, portals));
});
PaneviewReact.displayName = 'PaneviewComponent';
